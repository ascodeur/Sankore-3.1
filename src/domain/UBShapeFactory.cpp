#include "UBShapeFactory.h"
#include "UBFillingProperty.h"
#include "UBGraphicsEllipseItem.h"
#include "UBGraphicsPathItem.h"

#include "core/UBApplication.h"
#include "board/UBBoardController.h"
#include "board/UBBoardView.h"
#include "UBGraphicsScene.h"

UBShapeFactory::UBShapeFactory():
    mCurrentShape(NULL),
    mBoardView(NULL),
    mIsCreating(false),
    mIsPress(false),
    mIsRegularShape(true),
    mCurrentStrokeColor(Qt::black),
    mCurrentFillFirstColor(Qt::lightGray)
{

}


void UBShapeFactory::changeFillColor(bool ok)
{
    if(ok){
        UBGraphicsScene* scene = mBoardView->scene();

        QList<QGraphicsItem*> items = scene->selectedItems();

        for(int i = 0; i < items.size(); i++){
            UBShape * shape = dynamic_cast<UBShape*>(items.at(i));
            //UBGraphicsEllipseItem *ellipse = dynamic_cast<UBGraphicsEllipseItem*>(items.at(i));

            if(shape){
                shape->fillingProperty()->setFirstColor(QColor(128, 255, 100, 128));
                items.at(i)->update();
            }

        }
    }
}

void UBShapeFactory::init()
{
    mBoardView = UBApplication::boardController->controlView();

    connect(mBoardView, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(onMouseMove(QMouseEvent*)));
    connect(mBoardView, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(onMouseRelease(QMouseEvent*)));
    connect(mBoardView, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(onMousePress(QMouseEvent*)));

}


UBShape* UBShapeFactory::instanciateCurrentShape()
{
    switch (mShapeType) {
    case Ellipse:
    {
        mCurrentShape = new UBGraphicsEllipseItem();
        mCurrentShape->fillingProperty()->setFirstColor(Qt::red);
        break;
    }
    case Circle:
    {
        UBGraphicsEllipseItem * ellipse = new UBGraphicsEllipseItem();
        ellipse->setAsCircle();
        mCurrentShape = ellipse;
        break;
    }
    case Polygon:
    {
        UBGraphicsPathItem * pathItem = new UBGraphicsPathItem();
        pathItem->fillingProperty()->setFirstColor(mCurrentFillFirstColor);
        mCurrentShape = pathItem;
        break;
    }
    default:
        break;
    }

    return mCurrentShape;
}

void UBShapeFactory::createEllipse(bool create)
{
    if(create){
        mIsRegularShape = true;
        mIsCreating = true;
        mShapeType = Ellipse;
    }
}

void UBShapeFactory::createCircle(bool create)
{
    if(create){
        mIsRegularShape = true;
        mIsCreating = true;
        mShapeType = Circle;
    }
}

void UBShapeFactory::createPolygon(bool create)
{
    if(create){
        mIsRegularShape = false;
        mIsCreating = true;
        mShapeType = Polygon;
    }
}

void UBShapeFactory::onMouseMove(QMouseEvent *event)
{
    if(mIsCreating && mIsPress){
        QPointF cursorPosition = mBoardView->mapToScene(event->pos());

        if(mIsRegularShape){
            UBGraphicsEllipseItem* shape = dynamic_cast<UBGraphicsEllipseItem*>(mCurrentShape);
             QRectF rect = shape->rect();

             qreal w = cursorPosition.x() - rect.x();
             qreal h = cursorPosition.y() - rect.y();

             shape->setRect(QRectF(rect.x(), rect.y(), w, h));
        }else{

        }
    }
}

void UBShapeFactory::onMousePress(QMouseEvent *event)
{
    if(mIsCreating){
        mIsPress = true;

        QPointF cursorPosition = mBoardView->mapToScene(event->pos());

        if(mIsRegularShape){
            UBGraphicsEllipseItem* ellipse = dynamic_cast<UBGraphicsEllipseItem*>(instanciateCurrentShape());

            ellipse->setRect(QRectF(cursorPosition.x(), cursorPosition.y(), 0, 0));

            mBoardView->scene()->addItem(ellipse);
        }else{
            UBGraphicsPathItem* pathItem = dynamic_cast<UBGraphicsPathItem*>(mCurrentShape);
            if (mCurrentShape == NULL || pathItem == NULL)
            {
                pathItem = dynamic_cast<UBGraphicsPathItem*>(instanciateCurrentShape());
                mBoardView->scene()->addItem(pathItem);
            }
            pathItem->addPoint(cursorPosition);

            if (pathItem->isClosed())
            {
                // Si closed, terminer le dessin , en passant mIsCreating � false
                mIsCreating = false;
            }
        }
    }

}

void UBShapeFactory::onMouseRelease(QMouseEvent *event)
{
    mIsPress = false;

    if(mIsCreating){
        if (mIsRegularShape) // Les regularShapes finissent d'�tre dessin�es quand on relache la souris. Par contre les shapes "multi-points" (polygones) ne sont pas forcement finis (sauf si point press� == premier point).
        {
            mIsCreating = false;
            mCurrentShape = NULL;
        }
    }
}
