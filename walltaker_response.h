#pragma once
#include <string>
#include <optional>
#include <chrono>

struct WalltakerLink {
    std::optional<int> id;
    std::optional<std::chrono::system_clock::time_point> expires;
    std::string username;
    std::string terms;
    std::string blacklist;
    std::string post_url;
    std::string post_thumbnail_url;
    std::string post_description;
    std::optional<std::chrono::system_clock::time_point> created_at;
    std::optional<std::chrono::system_clock::time_point> updated_at;
    std::string set_by;
    std::string response_type;
    std::string response_text;
    std::optional<bool> online;
    std::string url;
};

// nlohmann::json support for WalltakerLink
#include <nlohmann/json.hpp>

inline void from_json(const nlohmann::json& j, WalltakerLink& link) {
    if (j.contains("id")) link.id = j.at("id").get<int>();
    if (j.contains("expires")) link.expires = std::nullopt; // TODO: parse ISO8601 to time_point
    link.username = j.contains("username") && !j.at("username").is_null() ? j.at("username").get<std::string>() : "";
    link.terms = j.contains("terms") && !j.at("terms").is_null() ? j.at("terms").get<std::string>() : "";
    link.blacklist = j.contains("blacklist") && !j.at("blacklist").is_null() ? j.at("blacklist").get<std::string>() : "";
    link.post_url = j.contains("post_url") && !j.at("post_url").is_null() ? j.at("post_url").get<std::string>() : "";
    link.post_thumbnail_url = j.contains("post_thumbnail_url") && !j.at("post_thumbnail_url").is_null() ? j.at("post_thumbnail_url").get<std::string>() : "";
    link.post_description = j.contains("post_description") && !j.at("post_description").is_null() ? j.at("post_description").get<std::string>() : "";
    if (j.contains("created_at")) link.created_at = std::nullopt; // TODO: parse ISO8601 to time_point
    if (j.contains("updated_at")) link.updated_at = std::nullopt; // TODO: parse ISO8601 to time_point
    link.set_by = j.contains("set_by") && !j.at("set_by").is_null() ? j.at("set_by").get<std::string>() : "";
    link.response_type = j.contains("response_type") && !j.at("response_type").is_null() ? j.at("response_type").get<std::string>() : "";
    link.response_text = j.contains("response_text") && !j.at("response_text").is_null() ? j.at("response_text").get<std::string>() : "";
    if (j.contains("online")) link.online = j.at("online").get<bool>();
    link.url = j.contains("url") && !j.at("url").is_null() ? j.at("url").get<std::string>() : "";
}