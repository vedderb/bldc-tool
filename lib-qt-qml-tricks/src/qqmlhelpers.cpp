#include "qqmlhelpers.h"

/*!
    \defgroup QT_QML_HELPERS Qt helper macros

    Brings a couple of macros that can help saving development time,
    by avoiding manual code duplication, often leading to heavy copy-and-paste,
    which is largely error-prone and not productive at all.
*/


/*!
    \def QML_WRITABLE_PROPERTY(type, name)
    \ingroup QT_QML_HELPERS
    \hideinitializer
    \details Creates a \c Q_PROPERTY that will be readable / writable from QML.

    \param type The C++ type of the property
    \param name The name for the property

    It generates for this goal :
    \code
        {type} m_{name}; // private member variable
        {type} get_{name} () const; // public getter method
        void set_{name} ({type}); // public setter slot
        void {name}Changed ({type}); // notifier signal
    \endcode

    \b Note : Any change from either C++ or QML side will trigger the notification.
*/


/*!
    \def QML_READONLY_PROPERTY(type, name)
    \ingroup QT_QML_HELPERS
    \hideinitializer
    \details Creates a \c Q_PROPERTY that will be readable from QML and writable from C++.

    \param type The C++ type of the property
    \param name The name for the property

    It generates for this goal :
    \code
        {type} m_{name}; // private member variable
        {type} get_{name} () const; // public getter method
        void update_{name} ({type}); // public setter method
        void {name}Changed ({type}); // notifier signal
    \endcode

    \b Note : Any change from C++ side will trigger the notification to QML.
*/


/*!
    \def QML_CONSTANT_PROPERTY(type, name)
    \ingroup QT_QML_HELPERS
    \hideinitializer
    \details Creates a \c Q_PROPERTY for a constant value exposed from C++ to QML.

    \param type The C++ type of the property
    \param name The name for the property

    It generates for this goal :
    \code
        {type} m_{name}; // private member variable
        {type} get_{name} () const; // public getter method
    \endcode

    \b Note : There is no change notifier because value is constant.
*/


/*!
    \def QML_ENUM_CLASS(name, ...)
    \ingroup QT_QML_HELPERS
    \hideinitializer
    \details Creates a class that contains a C++ enum that can be exposed to QML.

    \param name The name for the class
    \param ... The variadic list of values for the enum (comma-separated)

    It generates for this goal :
    \li The \c {name} C++ QObject-derived class
    \li The \c {name}::Type enumeration containing the values list
    \li The \c Q_ENUMS macro call to allow QML usage

    Example in use :
    \code
        QML_ENUM_CLASS (DaysOfWeek, Monday = 1, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday)
    \endcode

    \b Note : The QML registration using \c qmlRegisterUncreatableType() will still be needed.
*/
