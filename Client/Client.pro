<<<<<<< HEAD
QT       += core gui sql
=======
QT       += core gui network
>>>>>>> c01830f99e2b4ccd01168459f2184a5ad4aa4376

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
<<<<<<< HEAD
    list.cpp \
    main.cpp \
    mainwindow.cpp \
    signin.cpp \
    signon.cpp \
    verification.cpp

HEADERS += \
    list.h \
    mainwindow.h \
    signin.h \
    signon.h \
    verification.h

FORMS += \
    list.ui \
    mainwindow.ui \
    signin.ui \
    signon.ui \
    verification.ui
=======
    connection.cpp \
    main.cpp \
    signup.cpp \
    sr_service.cpp \
    text_msg.cpp

HEADERS += \
    connection.h \
    signup.h \
    sr_service.h \
    text_msg.h

FORMS += \
    login.ui \
    widget.ui
>>>>>>> c01830f99e2b4ccd01168459f2184a5ad4aa4376

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

<<<<<<< HEAD
RESOURCES += \
    resour.qrc
=======
DISTFILES += \
    img/1.jpg \
    img/bolder.png \
    img/clear.png \
    img/color.png \
    img/italic.png \
    img/save.png \
    img/underline.png
>>>>>>> c01830f99e2b4ccd01168459f2184a5ad4aa4376
