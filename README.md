About the project
=================

_The **QtQmlTricks** library contains useful classes and macros. It also has some test cases and examples._


###Why this project ?
All the people out there who have done / are doing a mixed C++ / QML project, have surely been thinking that it needs too much code on C++ side, and that this code is often not that easy to do right. So here is the solution :

**A nice library of classes and helpers that have been designed to simplify as much as possible the C++ side, so that it's finally not harder than QML side (which is already nice enough).**


###What does it bring at the moment ?

We have done a very simple library, which brings mainly some C++ **classes** :

* `QQmlObjectListModel` : a much nicer way to expose C++ list to QML than the quick & dirty `QList<QObject*>` property . Supports all the strong model features of `QAbstractListModel` while showing the simple and well know API of QList.
* `QQmlVariantListModel` : a dead-simple way to create a dynamic C++ list of any type and expose it to QML, way better than using a `QVariantList` property.

It also strouts a pack of helper C++ **macros** :

* `QML_WRITABLE_PROPERTY` : a macro that takes a type and a name, and creates automatically the member attribute, the public getter and setter, and the Qt signal for notifier, and allow use in QML by exposing a read/write `Q_PROPERTY`.
* `QML_READONLY_PROPERTY` : another macro that does almost the same as `QML_WRITABLE_PROPERTY` except that the property is not modifiable from the QML side, only C++ can access the setter.
* `QML_CONSTANT_PROPERTY` : a simplified version of the previous macros, that exposes a constant property with no getter and no setter, from C++ or QML side.
* `QML_LIST_PROPERTY` : a really handy macro to create a QML list property that maps to an internal QList of objects, without having to declare and implement all static function pointers...
* `QML_ENUM_CLASS` : a macro to declare a `QObject` class that only contains a `Q_ENUM` and can be exposed as is to QML.


**Note :** All these API are documented using _Doxygen_ so that one can easily generate HTML documentation or even Qt Help file (.qch).


###What are the plans for later ?

First we want to add a lot of new classes and helpers for common use.

Next, add some helpers and components for QML / JavaScript side too.

And maybe then, propose this for integration into standard Qt module QtCore, if it's really useful to people !


**Enjoy !**

