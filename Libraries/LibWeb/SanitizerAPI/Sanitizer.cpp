/*
 * Copyright (c) 2025, Miguel Sacristán Izcue <miguel_tete17@hotmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibWeb/SanitizerAPI/Sanitizer.h>

#include <LibJS/Runtime/Realm.h>
#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/WebIDL/ExceptionOr.h>

namespace Web::SanitizerAPI {

GC_DEFINE_ALLOCATOR(Sanitizer);

// https://wicg.github.io/sanitizer-api/#dom-sanitizer-constructor
WebIDL::ExceptionOr<GC::Ref<Sanitizer>> Sanitizer::construct_impl(JS::Realm& realm, Variant<Bindings::SanitizerConfig, Bindings::SanitizerPresets> configuration)
{
    // 1. If configuration is a SanitizerPresets string, then:
    if (configuration.has<Bindings::SanitizerPresets>()) {
        // 1. Assert: configuration is default.
        VERIFY(configuration.get<Bindings::SanitizerPresets>() == Bindings::SanitizerPresets::Default);

        // 2. TODO Set configuration to the built-in safe default configuration.
        configuration = Bindings::SanitizerConfig();
    }
    auto result = realm.create<Sanitizer>(realm);

    // 2. Let valid be the return value of set a configuration with configuration and true on this.
    auto const valid = result->set_a_configuration(configuration.get<Bindings::SanitizerConfig>(), AllowCommentsAndDataAttributes::Yes);

    // 3. If valid is false, then throw a TypeError.
    if (!valid)
        return WebIDL::SimpleException { WebIDL::SimpleExceptionType::TypeError, "Sanitizer configuration is not valid"_string };

    return result;
}

// https://wicg.github.io/sanitizer-api/#dom-sanitizer-setcomments
bool Sanitizer::set_comments(bool allow)
{
    // 1. Let configuration be this’s configuration.
    auto& configuration = m_configuration;

    // 2. TODO Assert: configuration is valid.

    // 3. If configuration["comments"] exists and configuration["comments"] equals allow, then return false;
    if (configuration.comments == allow)
        return false;

    // 4. Set configuration["comments"] to allow.
    m_configuration.comments = allow;

    // 5. Return true.
    return true;
}

// https://wicg.github.io/sanitizer-api/#dom-sanitizer-setdataattributes
bool Sanitizer::set_data_attributes(bool allow)
{
    // 1. Let configuration be this’s configuration.
    auto& configuration = m_configuration;

    // 2. TODO Assert: configuration is valid.

    // 3. If configuration["attributes"] does not exist, then return false.
    if (!configuration.attributes.has_value())
        return false;

    // 4. If configuration["dataAttributes"] equals allow, then return false.
    if (configuration.data_attributes == allow)
        return false;

    // 5. If allow is true:
    if (!allow) {
        // 1. Remove any items attr from configuration["attributes"] where attr is a custom data attribute.
        configuration.attributes.value().remove_all_matching(is_a_custom_data_attribute);

        // 2. If configuration["elements"] exists:
        if (auto* elements = configuration.elements.ptr(); elements) {
            // 1. For each element in configuration["elements"]:
            for (auto element : *elements) {
                // 1. If element[attributes] exists:
                if (auto* element_namespace = element.get_pointer<Bindings::SanitizerElementNamespaceWithAttributes>(); element_namespace) {
                    // 1. Remove any items attr from element[attributes] where attr is a custom data attribute.
                    element_namespace->attributes.value().remove_all_matching(is_a_custom_data_attribute);
                }
            }
        }
    }

    // 6. Set configuration["dataAttributes"] to allow.
    configuration.data_attributes = allow;

    // 7. Return true.
    return true;
}

Sanitizer::Sanitizer(JS::Realm& realm)
    : PlatformObject(realm)
{
}

void Sanitizer::initialize(JS::Realm& realm)
{
    WEB_SET_PROTOTYPE_FOR_INTERFACE(Sanitizer);
    Base::initialize(realm);
}

// https://wicg.github.io/sanitizer-api/#sanitizer-set-a-configuration
bool Sanitizer::set_a_configuration(Bindings::SanitizerConfig const& configuration, AllowCommentsAndDataAttributes)
{
    // 1. TODO Canonicalize configuration with allowCommentsPIsAndDataAttributes
    // 2. TODO If configuration is not valid, then return false.
    // 3. Set sanitizer’s configuration to configuration.
    m_configuration = configuration;

    // 4. Return true.
    return true;
}

// https://html.spec.whatwg.org/multipage/dom.html#custom-data-attribute
bool is_a_custom_data_attribute(SanitizerAttribute const& attribute)
{
    // TODO: It is not very clear in the spec what to do if the SanitizerAttribute is a SanitizerAttributeNamespace
    // A custom data attribute is an attribute in no namespace whose name starts with the string "data-",
    // has at least one character after the hyphen, is a valid attribute local name, and contains no ASCII upper alphas.
    return attribute.visit(
        [](Utf16String const& v) {
            return v.starts_with("data-"_utf16)
                && v.length_in_code_points() > 5
                && v.to_ascii_lowercase() == v;
        },
        [](Bindings::SanitizerAttributeNamespace const& v) {
            return !v.namespace_.has_value() && v.name.starts_with("data-"_utf16)
                && v.name.length_in_code_points() > 5
                && v.name.to_ascii_lowercase() == v.name;
        });
}

}
