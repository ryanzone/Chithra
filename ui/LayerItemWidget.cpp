#include "LayerItemWidget.hpp"

#include <QAction>
#include <QContextMenuEvent>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMenu>
#include <QMouseEvent>
#include <QToolButton>

LayerItemWidget::LayerItemWidget(const QString &name, bool visible,
                                 QWidget *parent)
    : QWidget(parent) {
  setFixedHeight(32);

  visibilityButton = new QToolButton(this);
  visibilityButton->setObjectName("visibilityButton");
  visibilityButton->setFixedSize(20, 20);
  visibilityButton->setIconSize(QSize(16, 16));

  if (visible)
    visibilityButton->setIcon(QIcon(":/icons/icons/eye-visible.svg"));
  else
    visibilityButton->setIcon(QIcon(":/icons/icons/eye-hidden.svg"));

  nameLabel = new QLabel(name, this);

  auto *layout = new QHBoxLayout(this);
  layout->setContentsMargins(4, 2, 4, 2);
  layout->setSpacing(6);

  layout->addWidget(visibilityButton);
  layout->addWidget(nameLabel);
  layout->addStretch();

  connect(visibilityButton, &QToolButton::clicked, this,
          &LayerItemWidget::visibilityToggled);

  setStyleSheet(R"(
        QWidget {
            background-color: #323232;
        }

        QWidget:hover {
            background-color: #3a3a3a;
        }

        QLabel {
            color: #e0e0e0;
        }

        QToolButton#visibilityButton {
            border: none;
            background: transparent;
        }
    )");
}

void LayerItemWidget::mousePressEvent(QMouseEvent *event) {
  emit clicked();
  QWidget::mousePressEvent(event);
}

void LayerItemWidget::contextMenuEvent(QContextMenuEvent *event) {
  QMenu menu(this);

  QAction *renameAction = menu.addAction("Rename Layer");

  QAction *deleteAction = menu.addAction("Delete Layer");

  QAction *selected = menu.exec(event->globalPos());

  if (selected == renameAction)
    emit renameRequested();

  if (selected == deleteAction)
    emit deleteRequested();
}