#include "maskwidget.h"
#include <QMutex>
#include <QEvent>
#include <QPainter>
#include <QApplication>
#include <QDebug>
#include <QTimer>

const int MinY = 26;

//MaskWidget* MaskWidget::m_pMask = nullptr;

//MaskWidget *MaskWidget::GetInstance()
//{
//    static QMutex s_Mutex;
//    if (nullptr == m_pMask)
//    {
//        QMutexLocker locker(&s_Mutex);
//        if (nullptr == m_pMask)
//        {
//            m_pMask = new MaskWidget;
//            m_pMask->setAttribute(Qt::WA_DeleteOnClose);
//        }
//    }
//    return m_pMask;
//}

void MaskWidget::SetMainWidget(QWidget *pWidgetMasked)
{
    if (m_pDlgToBeMasked != pWidgetMasked)
    {
        m_pDlgToBeMasked = pWidgetMasked;
        setGeometry(m_pDlgToBeMasked->geometry());
        pWidgetMasked->installEventFilter(this);
    }
}

//void MaskWidget::SetDialogNames(const QStringList &lstWidgetNames)
//{
//    if (m_lstNamesForDlgPrompt != lstWidgetNames)
//    {
//        m_lstNamesForDlgPrompt = lstWidgetNames;
//    }
//}

void MaskWidget::SetShowDialog(QWidget* dlg, bool bClickMaskHide)
{
    m_dialogMap[dlg] = bClickMaskHide;
}

void MaskWidget::RemoveDialog(QWidget* dlg)
{
    m_dialogMap.remove(dlg);
}

void MaskWidget::SetOpacity(float fOpacity)
{
    setWindowOpacity(fOpacity);
}

void MaskWidget::SetClickMaskHide(bool bClickMaskHide)
{
    m_bClickMaskHide = bClickMaskHide;
}

void MaskWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    //painter.setBrush(QBrush(QColor(0, 0, 0, 125)));
    painter.setPen(Qt::transparent);

    QRect rect = this->rect();
    rect.setWidth(rect.width());
    rect.setHeight(rect.height());

    //QPainterPath painterPath;
    //painterPath.addRoundedRect(rect, 15, 15);
    //painter.drawPath(painterPath);
    painter.fillRect(rect,QBrush(QColor(0, 0, 0, 125)));
    QWidget::paintEvent(event);
}

void MaskWidget::showEvent(QShowEvent *event)
{
    if (nullptr != m_pDlgToBeMasked)
    {
        //! 获取被遮盖的窗口的宽高
        QRect rectDlgMasked = m_pDlgToBeMasked->geometry();
        int nWidth = rectDlgMasked.width();
        int nHeight = rectDlgMasked.height();
        setGeometry(0, 0, nWidth, nHeight);
    }
    QWidget::showEvent(event);
}

void MaskWidget::resizeEvent(QResizeEvent* event)
{
    if(m_resizeTimer->isActive()){
        m_resizeTimer->stop();
    }
    m_resizeTimer->start();
    return QWidget::resizeEvent(event);
}

bool MaskWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (QEvent::Show == event->type())
    {
        //if (m_lstNamesForDlgPrompt.contains(obj->objectName()))
        QWidget* widget = qobject_cast<QWidget*>(obj);
        if(m_dialogMap.contains(widget))
        {
            this->setVisible(true);
            QRect maskRect = this->geometry();
            int posX = (maskRect.right() - widget->width()) / 2;
            int posY = (maskRect.bottom() - widget->height()) / 2;
            widget->move(posX, posY);
            //置顶上来
            this->raise();
            widget->raise();
        }
    }
    else if (QEvent::Hide == event->type())
    {
        //if (m_lstNamesForDlgPrompt.contains(obj->objectName()))
        QWidget* widgetObj = qobject_cast<QWidget*>(obj);
        if (m_dialogMap.contains(widgetObj))
        {
            this->setVisible(false);
        }
    }
    else if(QEvent::Resize == event->type() && obj == m_pDlgToBeMasked){
        //! 获取被遮盖的窗口的宽高
        QRect rectDlgMasked = m_pDlgToBeMasked->geometry();
        int nWidth = rectDlgMasked.width();
        int nHeight = rectDlgMasked.height();
        setGeometry(0, 0, nWidth, nHeight);
    }
    return QWidget::eventFilter(obj, event);
}

void MaskWidget::mousePressEvent(QMouseEvent *e)
{
    if(m_pDlgToBeMasked && m_pDlgToBeMasked->isVisible())
    {
        for (auto iter = m_dialogMap.begin(); iter != m_dialogMap.end(); iter++)
        {
            QWidget* widget = iter.key();
            if (!widget->isVisible()) {
                continue;
            }
            QPoint startPt = widget->mapToGlobal(QPoint(0, 0));
            const QRect& bgGlobalRect = QRect(startPt, QPoint(startPt.x() + widget->width(), startPt.y() + widget->height()));
            QPoint pt = QCursor::pos();
            if (!bgGlobalRect.contains(pt))
            {
                qInfo() << "click mask rect";
                if (iter.value())
                {
                    sigClicked(widget);
                    widget->hide();
                }
            }
        }
    }
    QWidget::mousePressEvent(e);
}

MaskWidget::MaskWidget(QWidget *parent)
    : QWidget(parent)
    , m_bClickMaskHide(true)
{
    setAttribute(Qt::WA_TranslucentBackground);
    m_resizeTimer = new QTimer(this);
    m_resizeTimer->setInterval(60);
    m_resizeTimer->setSingleShot(true);
    connect(m_resizeTimer, &QTimer::timeout, [this](){
        //resize widget
        for (auto iter = m_dialogMap.begin(); iter != m_dialogMap.end(); iter++)
        {
            QWidget* widget = iter.key();
            if (!widget->isVisible()) {
                continue;
            }
            QRect maskRect = this->geometry();
            int posX = (maskRect.right() - widget->width()) / 2;
            int posY = (maskRect.bottom() - widget->height()) / 2;
            if(posY < maskRect.top() + MinY){
                posY = maskRect.top() + MinY;
            }
            widget->move(posX, posY);
        }
    });
}

MaskWidget::~MaskWidget()
{
    //m_lstNamesForDlgPrompt.clear();
    m_dialogMap.clear();
}
