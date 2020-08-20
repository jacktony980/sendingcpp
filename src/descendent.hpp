
#pragma once

#include <memory>

namespace Kazv
{
    template<typename T>
    class Descendent
    {
        struct concept
        {
            virtual ~concept() = default;
            virtual const T *ptr() const = 0;
            virtual T *ptr() = 0;
            virtual std::unique_ptr<concept> clone() const = 0;
        };
        template<typename U>
        struct model : public concept
        {
            model(U x) : instance(std::move(x)) {}
            const T *ptr() const { return &instance; }
            T *ptr() { return &instance; }
            // or std::unique_ptr<model<U> >(new model<U>(U(instance))) if you do not have C++14
            std::unique_ptr<concept> clone() const { return std::make_unique<model<U> >(U(instance)); }
            U instance;
        };

        std::unique_ptr<concept> m_d;
    public:
        template<typename U>
        Descendent(U x) : m_d(std::make_unique<model<U> >(std::move(x))) {}

        Descendent(const Descendent & that) : m_d(std::move(that.m_d->clone())) {}
        Descendent(Descendent && that) : m_d(std::move(that.m_d)) {}

        Descendent & operator=(const Descendent &that) { Descendent t(that); *this = std::move(t); return *this; }
        Descendent & operator=(Descendent && that) { m_d = std::move(that.m_d); return *this; }

        const T *data() const { return m_d->ptr(); }
        const T *constData() const { return m_d->ptr(); }
        T *data() { return m_d->ptr(); }
        const T *operator->() const { return m_d->ptr(); }
        T *operator->() { return m_d->ptr(); }
    };
}
