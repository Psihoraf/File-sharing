#ifndef HPROTO_H
#define HPROTO_H

#include <cstdint>
#include <variant>
#include <type_traits>
#include <string>

#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#error "Big-endian not supported"
#endif

typedef uint32_t hproto_id_t;
typedef uint32_t string_len_t;

// Для совместимости с C++17
template<typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <typename T>
struct HProtoData {
};

template<>
struct HProtoData<std::string> {
    static  const hproto_id_t hproto_id = 0x0002;
    static  size_t hproto_size(const std::string &s) {
        return sizeof(string_len_t) + s.size();
    }
    static bool hproto_accepts_size(size_t s) {
        return s >= sizeof(string_len_t);
    }
    static void hproto_write(const std::string &s, void *data) {
        string_len_t size = s.size();
        std::memcpy(data, &size, sizeof(string_len_t));
        std::memcpy(static_cast<char*>(data) + sizeof(string_len_t), s.data(), size);
    }
    static std::string hproto_read(const void* data) {
        string_len_t size;
        std::memcpy(&size, data, sizeof(string_len_t));
        return std::string(static_cast<const char*>(data) + sizeof(string_len_t), size);
    }
};

#define HOTSPOT_SIZED_OBJECT(name, id, size)\
static_assert(sizeof(name) == size, "Bad hotspot type size: "#name);\
    template<>\
    struct HProtoData<name> {\
        static constexpr const hproto_id_t hproto_id = id;\
        static constexpr size_t hproto_size(const name &) {\
            return size;\
    }\
        static bool hproto_accepts_size(size_t s) {\
            return s == size;\
    }\
        static void hproto_write(const name &obj, void *data) {\
            std::memcpy(data, &obj, hproto_size(obj));\
    }\
        static name hproto_read(const void* data) {\
            return *reinterpret_cast<const name*>(data);\
    }\
};

#define HOTSPOT_EMPTY_OBJECT(name, id)\
template<>\
    struct HProtoData<name> {\
        static constexpr const hproto_id_t hproto_id = id;\
        static constexpr size_t hproto_size(const name &) {\
            return 0;\
    }\
        static bool hproto_accepts_size(size_t s) {\
            return s == 0;\
    }\
        static void hproto_write(const name &obj, void *data) {\
    }\
        static name hproto_read(const void* data) {\
            return name();\
    }\
};

template <typename Ts>
size_t hproto_size(std::variant<Ts> variant) {
    return std::visit([](const auto& value) {
        return sizeof(hproto_id_t) + HProtoData<remove_cvref_t<decltype(value)>>::hproto_size(value);
    }, variant);
}

template <typename Ts>
void hproto_write(std::variant<Ts> variant, void *data) {
    std::visit([data](const auto& value) {
        hproto_id_t id = HProtoData<remove_cvref_t<decltype(value)>>::hproto_id;
        std::memcpy(data, &id, sizeof(hproto_id_t));
        HProtoData<remove_cvref_t<decltype(value)>>::hproto_write(value, static_cast<char*>(data) + sizeof(hproto_id_t));
    }, variant);
}

template <typename T, typename... Ts>
bool hproto_try_variant_type(void *data, size_t size, hproto_id_t id, std::variant<Ts...> &var) {
    if (id != HProtoData<T>::hproto_id)
        return false;

    if (!HProtoData<T>::hproto_accepts_size(size)) {
        // qWarning не доступен здесь, уберем пока
        return false;
    }

    var.template emplace<T>(std::move(HProtoData<T>::hproto_read(data)));
    return true;
}

template <typename... Ts>
std::variant<std::monostate, Ts...> hproto_read(void *data, size_t size) {
    hproto_id_t id;
    std::memcpy(&id, data, sizeof(hproto_id_t));
    std::variant<std::monostate, Ts...> var;

    (hproto_try_variant_type<Ts, std::monostate, Ts...>(static_cast<char*>(data) + sizeof(hproto_id_t), size - sizeof(hproto_id_t), id, var) || ...);

    return var;
}

#endif // HPROTO_H
