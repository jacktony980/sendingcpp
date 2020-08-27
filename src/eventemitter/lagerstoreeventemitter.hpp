
#include <lager/store.hpp>
#include <lager/reader.hpp>

#include "eventinterface.hpp"

namespace Kazv
{
    class LagerStoreEventEmitter : public EventInterface
    {
        struct Model { KazvEvent curEvent; };
        struct Action { KazvEvent nextEvent; };
        static Model update(Model m, Action a) {
            return Model{a.nextEvent};
        }
    public:
        template<class EventLoop>
        LagerStoreEventEmitter(EventLoop &&loop) : m_store(
            lager::make_store<Action>(
                Model{},
                &update,
                std::forward<EventLoop>(loop))) {}
        ~LagerStoreEventEmitter() override = default;

        void emit(KazvEvent e) override {
            m_store.dispatch(Action{e});
        }

        class Watchable
        {
        public:
            Watchable(lager::reader<KazvEvent> r) : reader(r) {}
            template<class EventType, class Func>
            void after(Func &&func) {
                reader.watch(
                    [f=std::forward<Func>(func)](KazvEvent e) {
                        if (std::holds_alternative<EventType>(e)) {
                            f(std::get<EventType>(e));
                        }
                    });
            }

        private:
            lager::reader<KazvEvent> reader;
        };

        /**
         * An object you can watch for events.
         * ```
         * auto watchable = eventEmitter.watchable();
         * watchable.after<ReceivingRoomTimelineEvent>(
         *     [](auto &&e) { ... });
         * ```
         */
        Watchable watchable() {
            return lager::reader<KazvEvent>(
                m_store.zoom(lager::lenses::attr(&Model::curEvent)));
        }

    private:
        lager::store<Action, Model> m_store;
    };
}
