
# Change log

## Unreleased

- Support streaming uploads. https://gitlab.com/kazv/libkazv/-/merge_requests/1
- Fix ctx.dispatch() returning promises resolving to empty EffectStatus. https://gitlab.com/kazv/libkazv/-/commit/c7796a6ab8325773bf47fe83254a377204d6abcf
- Prevent leaking full file path when uploading to matrix server by using only basename for the request.
- Deal with timeline gaps properly. https://lily.kazv.moe/kazv/libkazv/-/merge_requests/1
- Record state events in timeline. https://lily.kazv.moe/kazv/libkazv/-/merge_requests/2
- Add support for streaming download. https://lily.kazv.moe/kazv/libkazv/-/merge_requests/3
- Add support for Boost.Serialization. https://lily.kazv.moe/kazv/libkazv/-/merge_requests/4
- Support encrypted attachments. https://lily.kazv.moe/kazv/libkazv/-/merge_requests/5
- Allow custom random generator with crypto. https://lily.kazv.moe/kazv/libkazv/-/merge_requests/6
- Support auto-discovery. https://lily.kazv.moe/kazv/libkazv/-/merge_requests/12
- Support profile API. https://lily.kazv.moe/kazv/libkazv/-/merge_requests/13

### Deprecated

- `makeDefaultEncryptedSdk()` is now deprecated. Use `makeDefaultSdkWithCryptoRandom()` instead.
