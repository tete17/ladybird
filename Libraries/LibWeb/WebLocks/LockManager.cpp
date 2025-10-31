/*
 * Copyright (c) 2025, Miguel Sacristán Izcue <miguel_tete17@hotmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "LibWeb/HTML/Scripting/Agent.h"

#include <LibWeb/WebLocks/LockManager.h>

#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/DOM/AbortSignal.h>
#include <LibWeb/HTML/Window.h>
#include <LibWeb/StorageAPI/StorageBottle.h>

namespace Web::WebLocks {

GC_DEFINE_ALLOCATOR(LockManager);

// https://www.w3.org/TR/web-locks/#dom-lockmanager-request
GC::Ref<WebIDL::Promise> LockManager::request(Utf16String const& name, GC::Ref<WebIDL::CallbackType>& callback)
{
    // NOTE: The rest of the implementation lives in the other overload
    // 1. If options was not passed, then let options be a new LockOptions dictionary with default members.
    return request(name, LockOptions {}, callback);
}

// https://www.w3.org/TR/web-locks/#dom-lockmanager-request
GC::Ref<WebIDL::Promise> LockManager::request(Utf16String const& name, LockOptions options, GC::Ref<WebIDL::CallbackType>& callback)
{
    auto& realm = this->realm();

    // NOTE: Only relevant for the other overload
    // 1. If options was not passed, then let options be a new LockOptions dictionary with default members.

    // 2. Let environment be this’s relevant settings object.
    auto& environment = HTML::relevant_settings_object(*this);

    // 3. If environment’s relevant global object’s associated Document is not fully active,
    //    then return a promise rejected with a "InvalidStateError" DOMException.
    if (as<HTML::Window>(environment.global_object()).associated_document().is_fully_active())
        return WebIDL::create_rejected_promise(realm, WebIDL::InvalidStateError::create(realm, "Document is not fully active"_utf16));

    // 4. Let manager be the result of obtaining a lock manager given environment.
    //    If that returned failure, then return a promise rejected with a "SecurityError" DOMException.
    auto manager = obtain_a_lock_manager(environment);
    if (!manager.has_value())
        return WebIDL::create_rejected_promise(realm, WebIDL::SecurityError::create(realm, "Can't access the Lock Manager"_utf16));

    // 5. If name starts with U+002D HYPHEN-MINUS (-),
    //    then return a promise rejected with a "NotSupportedError" DOMException.
    if (name.starts_with('-'))
        return WebIDL::create_rejected_promise(realm, WebIDL::NotSupportedError::create(realm, "Locks can start with -"_utf16));

    // 6. If both options["steal"] and options["ifAvailable"] are true,
    //    then return a promise rejected with a "NotSupportedError" DOMException.
    ASSERT(options.steal.has_value() && options.if_available.has_value());
    if (options.steal.value() && options.if_available.value())
        return WebIDL::create_rejected_promise(realm, WebIDL::NotSupportedError::create(realm, "Invalid Lock configuration"_utf16));

    // 7. If options["steal"] is true and options["mode"] is not "exclusive",
    //    then return a promise rejected with a "NotSupportedError" DOMException.
    ASSERT(options.steal.has_value() && options.mode.has_value());
    if (options.steal.value() && options.mode.value() == Bindings::LockMode::Exclusive)
        return WebIDL::create_rejected_promise(realm, WebIDL::NotSupportedError::create(realm, "Invalid Lock configuration"_utf16));

    // 8. If options["signal"] exists, and either of options["steal"] or options["ifAvailable"] is true,
    //    then return a promise rejected with a "NotSupportedError" DOMException.
    ASSERT(options.steal.has_value() && options.if_available.has_value());
    if (options.signal.has_value() && (options.steal.value() || options.if_available.value()))
        return WebIDL::create_rejected_promise(realm, WebIDL::NotSupportedError::create(realm, "Invalid Lock configuration"_utf16));

    // 9. If options["signal"] exists and is aborted, then return a promise rejected with options["signal"]'s abort reason.
    if (options.signal.has_value() && options.signal.value()->aborted())
        return WebIDL::create_rejected_promise(realm, options.signal.value()->reason());

    // 10. Let promise be a new promise.
    auto const promise = WebIDL::create_promise(realm);

    // [FIXME] 11. Request a lock with promise, the current agent, environment’s id, manager, callback, name, options["mode"], options["ifAvailable"], options["steal"], and options["signal"].

    // 12. Return promise.
    return promise;
}

LockManager::LockManager(JS::Realm& realm)
    : PlatformObject(realm)
{
}

void LockManager::initialize(JS::Realm& realm)
{
    WEB_SET_PROTOTYPE_FOR_INTERFACE(LockManager);
    Base::initialize(realm);
}

// https://www.w3.org/TR/web-locks/#obtain-a-lock-manager
Optional<Impl::LockManager&> obtain_a_lock_manager(HTML::EnvironmentSettingsObject& environment)
{
    // 1. Let map be the result of obtaining a local storage bottle map given environment and "web-locks".
    auto const map = StorageAPI::obtain_a_storage_bottle_map(StorageAPI::StorageType::Local, environment, StorageAPI::StorageEndpointType::WebLocks);

    // 2. If map is failure, then return failure.
    if (!map)
        return {};

    // 3. Let bottle be map’s associated storage bottle.
    // 4. Return bottle’s associated lock manager.
    return map->lock_manager();
}

}
