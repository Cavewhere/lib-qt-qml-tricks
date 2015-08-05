About the project
=================

_The **QtQmlTricks** library contains useful classes and macros. It also has some test cases and examples._


### Why this project ?
All the people out there who have done / are doing a mixed C++ / QML project, have surely been thinking that it needs too much code on C++ side, and that this code is often not that easy to do right. So here is the solution :

**A nice library of classes and helpers that have been designed to simplify as much as possible the C++ side, so that it's finally not harder than QML side (which is already nice enough).**


### What does it bring at the moment ?

We have done a very simple library, which brings mainly some C++ **classes** :

* `QQmlObjectListModel` : a much nicer way to expose C++ list to QML than the quick & dirty `QList<QObject*>` property . Supports all the strong model features of `QAbstractListModel` while showing the simple and well know API of QList.
* `QQmlVariantListModel` : a dead-simple way to create a dynamic C++ list of any type and expose it to QML, way better than using a `QVariantList` property.
* `QQmlSvgIconHelper` : a class that takes a SVG file as input, plus size/ratio information, and makes a PNG file in persistant cache as output. If additional color information other than transparent is provided, the opaque pixels of the output will be colorized with the given tint.
* `QQuickPolygon` : a simple QtQuick item that takes a list of points and draw them with a provided color.
* `QtCOBS` : a codec to create COBS-encoded QByteArray from raw data, and vice-versa.
* `QtBitStream` : a helper class to extract or inject custom amount of bits (between 1 and 64) in a `QByteArray` using a position cursor like `QDataStream` does for bytes...
* `QtJsonPath` : a nice way to retreive specific sub-values from a QJson*** class as a QVariant, by simply asking it by path.

It also strouts a pack of helper C++ **macros** :

* `QML_WRITABLE_PROPERTY` : a macro that takes a type and a name, and creates automatically the member attribute, the public getter and setter, and the Qt signal for notifier, and allow use in QML by exposing a read/write `Q_PROPERTY`.
* `QML_READONLY_PROPERTY` : another macro that does almost the same as `QML_WRITABLE_PROPERTY` except that the property is not modifiable from the QML side, only C++ can access the setter.
* `QML_CONSTANT_PROPERTY` : a simplified version of the previous macros, that exposes a constant property with no getter and no setter, from C++ or QML side.
* `QML_LIST_PROPERTY` : a really handy macro to create a QML list property that maps to an internal `QList` of objects, without having to declare and implement all static function pointers...
* `QML_ENUM_CLASS` : a macro to declare a `QObject` class that only contains a `Q_ENUM` and can be exposed as is to QML.

A nice set of JavaScript prototype modifications :

* `String` and `Array` classes extended to have API closer to `QString`/`QByteArray` and `QList`/`QVector` : addition of `startsWith`, `contains`, `at`, `size`, `append`, `prepend`, `insert`, `isEmpty`, ...

And a bunch of QML components :

* `RowContainer` : an horizontal layout, that positions its childrens side-by-side, setting their size in consequence of their implicit size hints, and using remaining space in the layout to distribute it between all items that expose a negative implicit width.
* `GridContainer` : a smart grid that dimensions itself according to the sum/max of its children's implicit size hints, and then distributes regularly the available space between all children, positioned against a col/row model.
* `WrapLeftRightContainer` : a simplified layout for one of the most common positioning scheme in UI, on the same line, put some items at left, the others at right. But it has extra intelligency, to wrap itslef it left/right items do not fit in the provided space.
* `ScrollContainer` : put a `Flickable` (or derived, e.g. `ListView`) in it to get vertical/horizontal scrollbars displayed around it (according to the flicking direction axis that are set).
* `IconTextButton` : a simple and customizable push button that can have either an icon, a label, or both. Colors, sizes, fonts and rounding are customizable.

**Note :** All these API are documented using _Doxygen_ so that one can easily generate HTML documentation or even Qt Help file (.qch).


### What are the plans for later ?

First we want to add a lot of new classes and helpers for common use.

Next, add more helpers and components for QML / JavaScript side too.

And maybe then, propose this for integration into standard Qt module QtCore, if it's really useful to people !

### How can I use it ?

Well, license here is pretty "super-open", not even copy-left or copy-right, just use it as you want, as it's the most practical to you :

* if you want to use it as GIT submodule and compile it in your app, do it ;
* if you prefer separate project as a shared library, do it ;
* if you need to modify the project to be able to integrate in you app (opensource or not), do it ;
* if you want to share you work on the library, thanks a lot, but if you don't, no problem ;
* if you think about some weird case I didn't talk about, well, do whatever you want, I don't need to know about it.

**Enjoy !**