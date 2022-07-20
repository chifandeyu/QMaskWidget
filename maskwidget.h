#ifndef MASKWIDGET_H
#define MASKWIDGET_H

#include <QWidget>
#include <QMap>

class MaskWidget : public QWidget
{
    Q_OBJECT
public:
    //static MaskWidget* GetInstance();
    //static void UnInstance();

    //! 设置被遮盖的控件
    void SetMainWidget(QWidget *pWidgetMasked);

    /*
     * \brief: 设置触发遮盖动作的控件名字
     *
     * \param[in]: lstWidgetNames 控件名字数组
     */
    //void SetDialogNames(const QStringList &lstWidgetNames);

    /*
     * \brief: 设置触发遮盖动作的弹窗
     * \param: dlg 需要覆盖的弹窗对象指针
     *         bClickMaskHide 点击周边阴影是否隐藏(默认隐藏)
     */
    void SetShowDialog(QWidget* dlg, bool bClickMaskHide = true);

    /*
     * \brief: 移除遮盖动作的弹窗
     * \param: dlg需要移除的弹窗对象指针
     */
    void RemoveDialog(QWidget* dlg);

    /*
     * \brief: 设置透明度
     *
     * \param[in]: fOpacity 透明度 取值范围0-1 1: 完全透明; 0: 完全不透明
     */
    void SetOpacity(float fOpacity);

    void SetClickMaskHide(bool bClickMaskHide);

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void showEvent(QShowEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual bool eventFilter(QObject *obj, QEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *e) override;

//private:
public:
    explicit MaskWidget(QWidget* wgt = nullptr);
    ~MaskWidget();

signals:
    void sigClicked(QWidget*);

private:
    static MaskWidget* m_pMask;
    QWidget* m_pDlgToBeMasked = nullptr;
    //QStringList m_lstNamesForDlgPrompt;
    QMap<QWidget*, bool> m_dialogMap;
    bool m_bClickMaskHide;
    QTimer* m_resizeTimer;
};

#endif // MASKWIDGET_H
