#include <QCoreApplication>
#include <UIKit/UIKit.h>
#include <QPointer>
#include <QtCore>
#include <QImage>
#include "qisystemmessenger.h"
#include "qiviewdelegate.h"

static bool isPad() {
    return ([UIDevice currentDevice].userInterfaceIdiom == UIUserInterfaceIdiomPad);
}

static QImage fromUIImage(UIImage* image) {
    QImage::Format format = QImage::Format_RGB32;

    CGColorSpaceRef colorSpace = CGImageGetColorSpace(image.CGImage);
    CGFloat width = image.size.width;
    CGFloat height = image.size.height;

    int orientation = [image imageOrientation];
    int degree = 0;

    switch (orientation) {
    case UIImageOrientationLeft:
        degree = -90;
        break;
    case UIImageOrientationDown: // Down
        degree = 180;
        break;
    case UIImageOrientationRight:
        degree = 90;
        break;
    }

    if (degree == 90 || degree == -90)  {
        CGFloat tmp = width;
        width = height;
        height = tmp;
    }

    QSize size(width,height);

    QImage result = QImage(size,format);

    CGContextRef contextRef = CGBitmapContextCreate(result.bits(),                 // Pointer to  data
                                                   width,                       // Width of bitmap
                                                   height,                       // Height of bitmap
                                                   8,                          // Bits per component
                                                   result.bytesPerLine(),              // Bytes per row
                                                   colorSpace,                 // Colorspace
                                                   kCGImageAlphaNoneSkipFirst |
                                                   kCGBitmapByteOrder32Little); // Bitmap info flags

    CGContextDrawImage(contextRef, CGRectMake(0, 0, width, height), image.CGImage);
    CGContextRelease(contextRef);

    if (degree != 0) {
        QTransform myTransform;
        myTransform.rotate(degree);
        result = result.transformed(myTransform,Qt::SmoothTransformation);
    }

    return result;
}

static QString fromNSUrl(NSURL* url) {
    QString result = QString::fromNSString([url absoluteString]);

    return result;
}

static UIViewController* rootViewController() {
    UIApplication* app = [UIApplication sharedApplication];
    return app.keyWindow.rootViewController;
}

static UIAlertView* alertView = nil;

static QIViewDelegate* alertViewDelegate = nil;

static void alertViewDismiss(int buttonIndex) {
    QString name = "alertViewClickedButtonAtIndex";
    QVariantMap data;
    data["buttonIndex"] = buttonIndex;
    QISystemMessenger* m_instance = QISystemMessenger::instance();
    QMetaObject::invokeMethod(m_instance,"received",Qt::DirectConnection,
                              Q_ARG(QString , name),
                              Q_ARG(QVariantMap,data));
    alertViewDelegate = nil;
    alertView = nil;
}

static bool alertViewCreate(QVariantMap& data) {
    Q_UNUSED(data);

    alertViewDelegate = [QIViewDelegate alloc];

    alertViewDelegate->alertViewClickedButtonAtIndex = ^(int buttonIndex) {
        alertViewDismiss(buttonIndex);
    };

    NSString* title = data["title"].toString().toNSString();
    NSString* message = data["message"].toString().toNSString();
    QStringList buttons = data["buttons"].toStringList();

    UIAlertView *alert = [UIAlertView alloc];
    alertView = alert;

    [alert initWithTitle:title
        message:message
        delegate:alertViewDelegate
        cancelButtonTitle:nil
        otherButtonTitles:nil
        ];

    for (int i = 0 ; i < buttons.size();i++) {
        NSString *btn = buttons.at(i).toNSString();
        [alert addButtonWithTitle:btn];
    }

    [alert show];

    return true;
}

static bool alertViewDismissWithClickedButtonIndex(QVariantMap& message) {
    if (alertView == nil) {
        return false;
    }

    int index = message["index"].toInt();
    bool animated = message["animated"].toBool();

    [alertView dismissWithClickedButtonIndex:index animated:animated];
    alertViewDismiss(index);
    return true;
}

static bool actionSheetCreate(QVariantMap& data) {
    static QIViewDelegate *delegate;

    delegate = [QIViewDelegate alloc];

    delegate->actionSheetClickedButtonAtIndex = ^(int buttonIndex) {
        QString name = "actionSheetClickedButtonAtIndex";
        QVariantMap data;
        data["buttonIndex"] = buttonIndex;
        QISystemMessenger* m_instance = QISystemMessenger::instance();
        QMetaObject::invokeMethod(m_instance,"received",Qt::DirectConnection,
                                  Q_ARG(QString , name),
                                  Q_ARG(QVariantMap,data));
    };

    delegate->actionSheetDidDismissWithButtonIndex = ^(int buttonIndex) {
        QString name = "actionSheetDidDismissWithButtonIndex";
        QVariantMap data;
        data["buttonIndex"] = buttonIndex;

        QISystemMessenger* m_instance = QISystemMessenger::instance();
        m_instance->sendMessage(name,data);
        delegate = nil;
    };

    NSString* title = nil;
    QString qTitle = data["title"].toString();
    if (!qTitle.isEmpty()) {
        title = qTitle.toNSString();
    }

    NSString* cancelButtonTitle = data["cancelButtonTitle"].toString().toNSString();
    QStringList buttons = data["otherButtonTitles"].toStringList();
    QRect rect = data["sourceRect"].value<QRect>();

    UIActionSheet* actionSheet = [UIActionSheet alloc];

    [actionSheet initWithTitle:title
        delegate:delegate
        cancelButtonTitle: nil
        destructiveButtonTitle:nil
        otherButtonTitles:nil];

    for (int i = 0 ; i < buttons.size();i++) {
        NSString *btn = buttons.at(i).toNSString();
        [actionSheet addButtonWithTitle:btn];
    }

    // Reference: http://stackoverflow.com/questions/1602214/use-nsarray-to-specify-otherbuttontitles

    [actionSheet addButtonWithTitle:cancelButtonTitle];

    actionSheet.cancelButtonIndex = buttons.size();

    if (isPad()) {
        [actionSheet showFromRect:CGRectMake(rect.x(),rect.y(),rect.width(),rect.height())
                inView:[rootViewController() view] animated:YES];

    } else {
        [actionSheet showInView:[UIApplication sharedApplication].keyWindow];
    }

    return true;
}

static UIImagePickerController* imagePickerController = 0;
static UIActivityIndicatorView* imagePickerControllerActivityIndicator = 0;

static bool imagePickerControllerPresent(QVariantMap& data) {

    UIApplication* app = [UIApplication sharedApplication];

    if (app.windows.count <= 0)
        return false;

    UIWindow* rootWindow = app.windows[0];
    UIViewController* rootViewController = rootWindow.rootViewController;

    int sourceType = data["sourceType"].toInt();
    bool animated = data["animated"].toBool();

    if (![UIImagePickerController isSourceTypeAvailable:(UIImagePickerControllerSourceType) sourceType]) {
        UIAlertView *myAlertView = [[UIAlertView alloc] initWithTitle:@"Error"
                          message:@"The operation is not supported in this device"
                          delegate:nil
                          cancelButtonTitle:@"OK"
                          otherButtonTitles: nil];
        [myAlertView show];
//        [myAlertView release];
        return false;
    }

    UIImagePickerController *picker = [[UIImagePickerController alloc] init];
    imagePickerController = picker;
    picker.sourceType = (UIImagePickerControllerSourceType) sourceType;

    static QIViewDelegate *delegate = 0;
    delegate = [QIViewDelegate alloc];

    delegate->imagePickerControllerDidFinishPickingMediaWithInfo = ^(UIImagePickerController *picker,
                                                                     NSDictionary* info) {
        Q_UNUSED(picker);

        QString name = "imagePickerControllerDisFinishPickingMetaWithInfo";
        QVariantMap data;

        data["mediaType"] = QString::fromNSString(info[UIImagePickerControllerMediaType]);
        data["mediaUrl"] = fromNSUrl(info[UIImagePickerControllerMediaURL]);
        data["referenceUrl"] = fromNSUrl(info[UIImagePickerControllerReferenceURL]);

        UIImage *chosenImage = info[UIImagePickerControllerEditedImage];
        if (!chosenImage) {
            chosenImage = info[UIImagePickerControllerOriginalImage];
        }

        if (!chosenImage) {
            qWarning() << "Image Picker: Failed to take image";
            name = "imagePickerControllerDidCancel";
        } else {
            QImage chosenQImage = fromUIImage(chosenImage);
            data["image"] = QVariant::fromValue<QImage>(chosenQImage);
        }

        QISystemMessenger* m_instance = QISystemMessenger::instance();

        QMetaObject::invokeMethod(m_instance,"received",Qt::DirectConnection,
                                  Q_ARG(QString , name),
                                  Q_ARG(QVariantMap,data));

        delegate = nil;
    };

    delegate->imagePickerControllerDidCancel = ^(UIImagePickerController *picker) {
        Q_UNUSED(picker);

        QString name = "imagePickerControllerDidCancel";
        QVariantMap data;
        QISystemMessenger* m_instance = QISystemMessenger::instance();
        QMetaObject::invokeMethod(m_instance,"received",Qt::DirectConnection,
                                  Q_ARG(QString , name),
                                  Q_ARG(QVariantMap,data));

        delegate = nil;
    };

    picker.delegate = delegate;

    imagePickerControllerActivityIndicator = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
    imagePickerControllerActivityIndicator.center = picker.view.center;
    [picker.view addSubview:imagePickerControllerActivityIndicator];

    [rootViewController presentViewController:picker animated:animated completion:NULL];

    return true;
}

bool imagePickerControllerDismiss(QVariantMap& data) {
    Q_UNUSED(data);
    if (!imagePickerController)
        return false;

    bool animated = data["animated"].toBool();

    [imagePickerController dismissViewControllerAnimated:animated completion:NULL];
//    [imagePickerController release];

//    [imagePickerControllerActivityIndicator release];

    imagePickerController = 0;
    imagePickerControllerActivityIndicator = 0;
    return true;
}

bool imagePickerControllerSetIndicator(QVariantMap& data) {
    if (!imagePickerControllerActivityIndicator)
        return false;

    bool active = data["active"].toBool();

    if (active) {
        [imagePickerControllerActivityIndicator startAnimating];
    } else {
        [imagePickerControllerActivityIndicator stopAnimating];
    }

    return true;
}


static bool applicationSetStatusBarStyle(QVariantMap& data) {
    if (!data.contains("style")) {
        qWarning() << "applicationSetStatusBarStyle: Missing argument";
        return false;
    }

    int style = data["style"].toInt();
    [[UIApplication sharedApplication] setStatusBarStyle:(UIStatusBarStyle) style];
    return true;
}

static bool applicationSetStatusBarHidden(QVariantMap& data) {
    bool hidden = data["hidden"].toBool();
    int animation = data["animation"].toInt();

    [[UIApplication sharedApplication] setStatusBarHidden:(bool) hidden withAnimation:(UIStatusBarAnimation) animation];
    return true;
}

static UIActivityIndicatorView* activityIndicator = 0;

static bool activityIndicatorStartAniamtion(QVariantMap& data) {
    Q_UNUSED(data);
    if (!activityIndicator) {
        int style = data["style"].toInt();

        UIViewController* rootView = rootViewController();

        activityIndicator = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:(UIActivityIndicatorViewStyle) style];
        activityIndicator.center = rootView.view.center;
        activityIndicator.hidesWhenStopped = YES;

        [rootView.view addSubview:activityIndicator];
    }

    [activityIndicator startAnimating];
    return true;
}

static bool activityIndicatorStopAnimation(QVariantMap& data) {
    Q_UNUSED(data);

    if (!activityIndicator) {
        return false;
    }

    [activityIndicator stopAnimating];
//    [activityIndicator release];
    activityIndicator = 0;
    return true;
}

class QISystemUtilsRegisterHelper {
public:
    QISystemUtilsRegisterHelper() {
        QISystemMessenger* messenger = QISystemMessenger::instance();

        messenger->registerMessageHandler("alertViewCreate",alertViewCreate);
        messenger->registerMessageHandler("alertViewDismissWithClickedButtonIndex", alertViewDismissWithClickedButtonIndex);

        messenger->registerMessageHandler("applicationSetStatusBarStyle",applicationSetStatusBarStyle);
        messenger->registerMessageHandler("applicationSetStatusBarHidden",applicationSetStatusBarHidden);

        messenger->registerMessageHandler("actionSheetCreate",actionSheetCreate);
        messenger->registerMessageHandler("imagePickerControllerPresent",imagePickerControllerPresent);
        messenger->registerMessageHandler("imagePickerControllerDismiss",imagePickerControllerDismiss);
        messenger->registerMessageHandler("imagePickerControllerSetIndicator",imagePickerControllerSetIndicator);

        messenger->registerMessageHandler("activityIndicatorStartAnimation",activityIndicatorStartAniamtion);
        messenger->registerMessageHandler("activityIndicatorStopAnimation",activityIndicatorStopAnimation);
    }

};

static QISystemUtilsRegisterHelper registerHelper;
