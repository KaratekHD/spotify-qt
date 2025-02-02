#include "jsonutils.hpp"

QJsonValue JsonUtils::getProperty(const QJsonObject &json, const QStringList &names)
{
	for (auto &name : names)
		if (json.contains(name))
			return json[name];

	return QJsonValue();
}

nlohmann::json JsonUtils::toJson(const QJsonValue &json)
{
	auto document = (json.isObject()
		? QJsonDocument(json.toObject())
		: json.isArray()
			? QJsonDocument(json.toArray())
			: QJsonDocument()).toJson().toStdString();

	return document.empty()
		? nlohmann::json()
		: nlohmann::json::parse(document);
}
