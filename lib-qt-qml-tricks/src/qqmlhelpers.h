#ifndef QQMLHELPERS_H
#define QQMLHELPERS_H
#include <QObject>

#define QML_WRITABLE_PROPERTY(type, name) \
    protected: \
        Q_PROPERTY (type name READ get_##name WRITE set_##name NOTIFY name##Changed) \
    private: \
        type m_##name; \
    public: \
        type get_##name () const { \
            return m_##name ; \
        } \
    public Q_SLOTS: \
        bool set_##name (type name) { \
            bool ret = false; \
            if ((ret = m_##name != name)) { \
                m_##name = name; \
                emit name##Changed (m_##name); \
            } \
            return ret; \
        } \
    Q_SIGNALS: \
        void name##Changed (type name); \
    private:

#define QML_READONLY_PROPERTY(type, name) \
    protected: \
        Q_PROPERTY (type name READ get_##name NOTIFY name##Changed) \
    private: \
        type m_##name; \
    public: \
        type get_##name () const { \
            return m_##name ; \
        } \
    protected: \
        bool update_##name (type name) { \
            bool ret = false; \
            if ((ret = m_##name != name)) { \
                m_##name = name; \
                emit name##Changed (m_##name); \
            } \
            return ret; \
        } \
    Q_SIGNALS: \
        void name##Changed (type name); \
    private:

#define QML_CONSTANT_PROPERTY(type, name) \
    protected: \
        Q_PROPERTY (type name READ get_##name CONSTANT) \
    private: \
        type m_##name; \
    public: \
        type get_##name () const { \
            return m_##name ; \
        } \
    private:

#define QML_LIST_PROPERTY(CLASS, NAME, TYPE) \
    public: \
        static int NAME##_count (QQmlListProperty<TYPE> * prop) { \
            CLASS * instance = qobject_cast<CLASS *> (prop->object); \
            return (instance != NULL ? instance->m_##NAME.count () : 0); \
        } \
        static void NAME##_clear (QQmlListProperty<TYPE> * prop) { \
            CLASS * instance = qobject_cast<CLASS *> (prop->object); \
            if (instance != NULL) { \
                instance->m_##NAME.clear (); \
            } \
        } \
        static void NAME##_append (QQmlListProperty<TYPE> * prop, TYPE * obj) { \
            CLASS * instance = qobject_cast<CLASS *> (prop->object); \
            if (instance != NULL && obj != NULL) { \
                instance->m_##NAME.append (obj); \
            } \
        } \
        static TYPE * NAME##_at (QQmlListProperty<TYPE> * prop, int idx) { \
            CLASS * instance = qobject_cast<CLASS *> (prop->object); \
            return (instance != NULL ? instance->m_##NAME.at (idx) : NULL); \
        } \
        QList<TYPE *> get_##NAME##s (void) const { \
            return m_##NAME; \
        } \
    private: \
        QList<TYPE *> m_##NAME;

#define QML_ENUM_CLASS(name, ...) \
    class name : public QObject { \
        Q_GADGET \
    public: \
        enum Type { __VA_ARGS__ }; \
        Q_ENUMS (Type) \
    };

// wrapper properties
#define QML_WRITABLE_PROPERTY_WRAPPER(type, name, data, baseType) \
    protected: \
        Q_PROPERTY (type name READ get_##name WRITE set_##name NOTIFY name##Changed) \
    public: \
        type get_##name () const { \
            return data ; \
        } \
    public Q_SLOTS: \
        bool set_##name (type name) { \
            bool ret = false; \
            if ((ret = data != (baseType)name)) { \
                data = name; \
                emit name##Changed (data); \
            } \
            return ret; \
        } \
    Q_SIGNALS: \
        void name##Changed (type name); \
    private:

#define QML_WRITABLE_PROPERTY_W(type, name, data) \
    protected: \
        Q_PROPERTY (type name READ get_##name WRITE set_##name NOTIFY name##Changed) \
    public: \
        type get_##name () const { \
            return data ; \
        } \
    public Q_SLOTS: \
        bool set_##name (type name) { \
            bool ret = false; \
            if ((ret = data != name)) { \
                data = name; \
                emit name##Changed (data); \
            } \
            return ret; \
        } \
    Q_SIGNALS: \
        void name##Changed (type name); \
    private:

#define QML_WRITABLE_ENUM_PROPERTY_W(type, name, datatype, data) \
    protected: \
        Q_ENUMS(type) \
        Q_PROPERTY (type name READ get_##name WRITE set_##name NOTIFY name##Changed) \
    public: \
        type get_##name () const { \
            return (type)data ; \
        } \
        datatype get_src_##name () const { \
            return data ; \
        } \
    public Q_SLOTS: \
        bool set_##name (type name) { \
            return set_##name((datatype) name); \
        } \
        bool set_##name (datatype name) { \
            bool ret = false; \
            if ((ret = data != name)) { \
                data = name; \
                emit name##Changed ((type)data); \
            } \
            return ret; \
        } \
    Q_SIGNALS: \
        void name##Changed (type name); \
    private:

#define QML_WRITEONLY_PROPERTY(type, name) \
    protected: \
        Q_PROPERTY (type name READ get_##name WRITE set_##name) \
    private: \
        type m_##name; \
        const type& get_##name() const {return m_##name;} \
    public Q_SLOTS: \
        void set_##name (type name) { \
                m_##name = name; \
        } \
    private:

class QmlProperty : public QObject { Q_OBJECT }; // NOTE : to avoid "no suitable class found" MOC note

#endif // QQMLHELPERS_H
