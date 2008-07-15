/***************************************************************************
*   Copyright (C) 2006-2008 by Thomas Schweitzer                          *
*   thomas-schweitzer(at)arcor.de                                         *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License version 2.0 as   *
*   published by the Free Software Foundation.                            *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program in the file LICENSE.GPL; if not, write to the *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/
#include <QtGui>
#include "AboutDialogGraphicsView.h"

#if QT_VERSION >= 0x040400

AboutDialogGraphicsView::AboutDialogGraphicsView(AboutDialog *aboutDialog, QWidget *parent) : QGraphicsView(parent) {
    this->parent = parent;
    setWindowFlags(Qt::SplashScreen);
    QRect newGeometry = QRect( -1, -1, QApplication::desktop()->rect().width()+2, QApplication::desktop()->rect().height()+2 );
    setGeometry( newGeometry );
    //setTransform(QTransform()
    //    .translate(newGeometry.width() / 2, newGeometry.height() / 2));
    //setWindowState(Qt::WindowFullScreen);

    this->aboutDialog = aboutDialog;

    firstRunOfAnimation = true;

    scene = new QGraphicsScene(this);
    setSceneRect( newGeometry );
    aboutDialogAsLabel = new QLabel(this, Qt::SplashScreen);
    aboutDialogAsLabel->setPixmap( QPixmap::grabWidget(aboutDialog) );
    graphicsProxyWidget = scene->addWidget(aboutDialogAsLabel, Qt::SplashScreen);
    //graphicsProxyWidget->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    //graphicsProxyWidget->setGeometry(newGeometry);
    //graphicsProxyWidget->setWindowTitle( aboutDialog->windowTitle() );

    setScene( scene );
    setRenderHint(QPainter::Antialiasing);

    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    connect(aboutDialog, SIGNAL(finished(int)), this, SLOT(hide()));

    //setWindowOpacity(0.9);
    
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    timeLine = new QTimeLine(1000, this);
    timeLine->setFrameRange(270, 0);
    //timeLine->setUpdateInterval(10);
    //timeLine->setCurveShape(QTimeLine::EaseInCurve);
    connect(timeLine, SIGNAL(frameChanged(int)), this, SLOT(updateStep(int)));

    windowBorderWidth = 0;
    windowTitleBarWidth = parent->geometry().y() - parent->y();
	
	originalPixmap = QPixmap::grabWindow(QApplication::desktop()->screen()->winId());
	qDebug("pixmap width %d, numScreens = %d", originalPixmap.size().width(), QApplication::desktop()->availableGeometry().width());
}

AboutDialogGraphicsView::~AboutDialogGraphicsView(void) {
}


void AboutDialogGraphicsView::show() {
    QPixmap originalPixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
    setBackgroundBrush(originalPixmap);

    aboutDialogAsLabel->setPixmap( QPixmap::grabWidget(aboutDialog) );
    graphicsProxyWidget->setGeometry( aboutDialog->geometry() );
    if ( firstRunOfAnimation ) {
        graphicsProxyWidget->setPos( parent->geometry().x()+(parent->geometry().width()-graphicsProxyWidget->geometry().width()) / 2, parent->y()+windowTitleBarWidth);
    } 
    else {
        graphicsProxyWidget->setPos( parent->geometry().x()+(parent->geometry().width()-graphicsProxyWidget->geometry().width()) / 2+windowBorderWidth, parent->y()+windowTitleBarWidth);
    }

    QRectF r = graphicsProxyWidget->boundingRect();
    graphicsProxyWidget->setTransform(QTransform()
        .translate(r.width() / 2, -windowTitleBarWidth)
        .rotate(270, Qt::XAxis)
        //.rotate(90, Qt::YAxis)
        //.rotate(5, Qt::ZAxis)
        //.scale(1 + 1.5 * step, 1 + 1.5 * step)
        .translate(-r.width() / 2, windowTitleBarWidth));

    graphicsProxyWidget->show();
    //aboutDialogAsLabel->show();
    QGraphicsView::show();

    connect(timeLine, SIGNAL(finished()), this, SLOT(showAboutDialog()));
    timeLine->setDirection(QTimeLine::Forward);
    timeLine->start();
}


void AboutDialogGraphicsView::updateStep(int step)
{
    QRectF r = graphicsProxyWidget->boundingRect();
    graphicsProxyWidget->setTransform(QTransform()
        .translate(r.width() / 2, -windowTitleBarWidth)
        .rotate(step, Qt::XAxis)
        //.rotate(step, Qt::YAxis)
        //.rotate(step * 5, Qt::ZAxis)
        //.scale(1 + 1.5 * step, 1 + 1.5 * step)
        .translate(-r.width() / 2, windowTitleBarWidth));
    //update();
}


void AboutDialogGraphicsView::showAboutDialog() {
    //hide();
    disconnect(timeLine, SIGNAL(finished()), this, SLOT(showAboutDialog()));
    graphicsProxyWidget->hide();
    aboutDialog->move( parent->geometry().x()+(parent->geometry().width()-graphicsProxyWidget->geometry().width())/2, parent->y()+windowTitleBarWidth );
    aboutDialog->exec();
}


void AboutDialogGraphicsView::hide() {
    aboutDialogAsLabel->setPixmap( QPixmap::grabWidget(aboutDialog) );
    graphicsProxyWidget->setGeometry( aboutDialog->geometry() );
    //if ( firstRunOfAnimation ) {
        firstRunOfAnimation = false;
    //    graphicsProxyWidget->setPos( parent->geometry().x()+(parent->geometry().width()-graphicsProxyWidget->geometry().width()) / 2, parent->y());
    //} 
    //else {
        graphicsProxyWidget->setPos( parent->geometry().x()+(parent->geometry().width()-graphicsProxyWidget->geometry().width()) / 2+windowBorderWidth, parent->y()+windowTitleBarWidth);
    //}

    QRectF r = graphicsProxyWidget->boundingRect();
    graphicsProxyWidget->setTransform(QTransform()
        .translate(r.width() / 2, -windowTitleBarWidth)
        .rotate(0, Qt::XAxis)
        //.rotate(90, Qt::YAxis)
        //.rotate(5, Qt::ZAxis)
        //.scale(1 + 1.5 * step, 1 + 1.5 * step)
        .translate(-r.width() / 2, windowTitleBarWidth));

    graphicsProxyWidget->show();
    //aboutDialogAsLabel->show();
    QGraphicsView::show();

    connect(timeLine, SIGNAL(finished()), this, SLOT(hideReally()));
    timeLine->setDirection(QTimeLine::Backward);
    timeLine->start();
}

void AboutDialogGraphicsView::hideReally() {
    disconnect(timeLine, SIGNAL(finished()), this, SLOT(hideReally()));
    QGraphicsView::hide();
}


void AboutDialogGraphicsView::setScreenshotPixmap(const QPixmap &screenShot) {
    originalPixmap = screenShot;
}


#endif // #if QT_VERSION >= 0x040400