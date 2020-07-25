#include "utils.hpp"
#include "icon.hpp"

bool Utils::darkBackground = false;

QJsonValue Utils::getProperty(const QJsonObject &json, const QStringList &names)
{
	for (auto &name : names)
		if (json.contains(name))
			return json[name];
	return QJsonValue();
}

QTreeWidgetItem *Utils::treeItemWithChildren(
	QTreeWidget *tree, const QString &name,
	const QString &toolTip, const QStringList &childrenItems)
{
	auto item = new QTreeWidgetItem(tree, {name});
	item->setToolTip(0, toolTip);
	for (auto &child : childrenItems)
		item->addChild(new QTreeWidgetItem(item, {child}));

	return item;
}

void Utils::applyPalette(Palette palette)
{
	QPalette p;
	switch (palette)
	{
		case paletteApp: p = QApplication::palette();
			break;

		case paletteStyle: p = QApplication::style()->standardPalette();
			break;

		case paletteDark: p = DarkPalette();
			break;
	}

	QApplication::setPalette(p);
}

QPixmap Utils::mask(const QPixmap &source, Utils::MaskShape shape, const QVariant &data)
{
	if (source.isNull())
		return source;

	auto img = source.toImage().convertToFormat(QImage::Format_ARGB32);
	QImage out(img.size(), QImage::Format_ARGB32);
	out.fill(Qt::GlobalColor::transparent);
	QPainter painter(&out);
	painter.setOpacity(0);
	painter.setBrush(Qt::white);
	painter.setPen(Qt::NoPen);
	painter.drawImage(0, 0, img);
	painter.setOpacity(1);
	QPainterPath path(QPointF(0, 0));

	QPolygonF polygon;
	switch (shape)
	{
		case Utils::MaskShape::App:

			polygon << QPointF(img.width() / 4, 0)
					<< QPointF(img.width(), 0)
					<< QPointF(img.width(), (img.height() / 4) * 3)
					<< QPointF((img.width() / 4) * 3, img.height())
					<< QPointF(0, img.height())
					<< QPointF(0, img.height() / 4);
			break;

		case Utils::MaskShape::Pie:
			switch (data.toInt() / 25)
			{
				case 0:
					polygon = QPolygonF(QRectF(
						img.width() / 2, 0,
						img.width() / 2, img.height() / 2));
					break;

				case 1:
					polygon = QPolygonF(QRectF(
						img.width() / 2, 0,
						img.width() / 2, img.height()));
					break;

				case 2:
					polygon << QPointF(img.width() / 2, 0)
							<< QPointF(img.width() / 2, img.height() / 2)
							<< QPointF(0, img.height() / 2)
							<< QPointF(0, img.height())
							<< QPointF(img.width(), img.height())
							<< QPointF(img.width(), 0);
					break;

				case 3:
					polygon = QPolygonF(QRectF(
						0, 0, img.width(), img.height()));
					break;
			}
			break;
	}

	path.addPolygon(polygon);
	painter.setClipPath(path);
	painter.drawImage(0, 0, img);
	return QPixmap::fromImage(out);
}

QWidget *Utils::layoutToWidget(QLayout *layout)
{
	auto widget = new QWidget();
	widget->setLayout(layout);
	return widget;
}

QString Utils::formatTime(int ms)
{
	auto duration = QTime(0, 0).addMSecs(ms);
	return QString("%1:%2")
		.arg(duration.minute())
		.arg(duration.second() % 60, 2, 10, QChar('0'));
}

QGroupBox *Utils::createGroupBox(QVector<QWidget*> &widgets, QWidget *parent)
{
	auto group = new QGroupBox(parent);
	auto layout = new QVBoxLayout();
	for (auto &widget : widgets)
		layout->addWidget(widget);
	group->setLayout(layout);
	return group;
}

QAction *Utils::createMenuAction(
	const QString &iconName, const QString &text,
	QKeySequence::StandardKey shortcut)
{
	auto action = new QAction(Icon::get(iconName), text);
	if (shortcut != QKeySequence::UnknownKey)
		action->setShortcut(QKeySequence(shortcut));
	return action;
}

QTreeWidgetItem *Utils::treeItem(QTreeWidget *tree, const QString &key, const QString &value)
{
	return new QTreeWidgetItem(tree, {
		key, value
	});
}