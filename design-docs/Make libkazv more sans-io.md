
# Make libkazv more sans-io

## Current problem

The library is not sans-io enough. For example, although we made
`ClientModel` return its jobs and triggers in the reducer, it is
not easy to keep this pattern once we made our way into E2EE.

Sending encrypted message itself must be done in at least 3 requests,
and there is a dependency relationship between them:

    Send Megolm encrypted event
    Claim keys if needed -> Send session key encrypted with olm

We cannot get such dependency flows in sans-io. We need to do it
in the IO-integration layer. But how?

## sans-io patterns

Ideally we should have:

```c++
Promise submitJobs(jobs)
{
    auto promises = ContainerT{};
    for (auto job : jobs) {
        promises += jobHandler.submit(job)
            .then([=](Response r) { ctx.dispatch(ProcessResponseAction{r}); });
    }

    auto p = Promise::all(promises);
}

Promise Client::doSomething()
{
    const auto client = +clientReader;
    auto [jobs, triggers] = client.perform(Action);

    return submitJobs(jobs);
}
```

## Problem with that

But there is difficulty -- `client.perform(Action)` may need to
modify `client`, for example, encrypting a message will change
the state of `crypto`. To solve this problem, one can propose:

```
Promise Client::doSomething()
{
    const auto client = +clientReader;
    auto newClient = client.perform(Action);
    auto jobs = newClient.popAllJobs();
    auto triggers = newClient.popAllTriggers();

    ctx.dispatch(SetClient(newClient));

    return submitJobs(jobs);
}
```

But it is definitely not thread-safe because between we
fetch the value in `clientReader` and `SetClient` is actually
executed, the client may already be changed, resulting in a
state conflict.

It may be possible then, to wrap the `perform` into another
action, and do:

```
??? Client::doSomething()
{
    ctx.dispatch(PerformAction{Action});
}

ClientResult updateClient(Client c, PerformAction a)
{
    auto newClient = client.perform(Action);
    auto jobs = newClient.popAllJobs();
    auto triggers = newClient.popAllTriggers();

    return { c, [=](auto &&ctx) { submitJobs(jobs); } };
}
```

But where goes the `Promise`? As seen above, we have
no way to represent "when the submitted jobs finished."

Using the event loop directly?

```
Promise Client::doSomething()
{
    return eventLoop.post([=]() {
        const auto client = +clientReader;
        auto newClient = client.perform(Action);
        auto jobs = newClient.popAllJobs();
        auto triggers = newClient.popAllTriggers();

        ctx.dispatch(SetClient(newClient));

        return submitJobs(jobs);
    });
}
```

This actually looks exactly the same as the current
implementation, except that the current one is done
in the `SdkModel` reducer:

```
SdkResult updateSdk(SdkModel s, SdkAction a)
{
    if (a.is<ClientAction>()) {
        auto newClient = updateClient(s.client, a.to<ClientAction>());
        auto jobs = newClient.popAllJobs();
        auto triggers = newClient.popAllTriggers();
        return { {newClient},
                 [=](auto &&ctx) {
                     submitJobs(jobs);
                     emit(triggers);
                 }
               };
    }
}
```
