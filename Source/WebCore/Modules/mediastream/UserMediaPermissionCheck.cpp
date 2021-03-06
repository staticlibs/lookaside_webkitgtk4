/*
 * Copyright (C) 2015-2016 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "config.h"
#include "UserMediaPermissionCheck.h"

#if ENABLE(MEDIA_STREAM)

#include "Document.h"
#include "ExceptionCode.h"
#include "Frame.h"
#include "JSMediaDeviceInfo.h"
#include "MainFrame.h"
#include "RealtimeMediaSourceCenter.h"
#include "SecurityOrigin.h"
#include "UserMediaController.h"

namespace WebCore {

Ref<UserMediaPermissionCheck> UserMediaPermissionCheck::create(Document& document, UserMediaPermissionCheckClient& client)
{
    return adoptRef(*new UserMediaPermissionCheck(document, client));
}

UserMediaPermissionCheck::UserMediaPermissionCheck(ScriptExecutionContext& context, UserMediaPermissionCheckClient& client)
    : ContextDestructionObserver(&context)
    , m_client(&client)
{
}

UserMediaPermissionCheck::~UserMediaPermissionCheck()
{
}

SecurityOrigin* UserMediaPermissionCheck::userMediaDocumentOrigin() const
{
    if (m_scriptExecutionContext)
        return m_scriptExecutionContext->securityOrigin();

    return nullptr;
}

SecurityOrigin* UserMediaPermissionCheck::topLevelDocumentOrigin() const
{
    if (!m_scriptExecutionContext)
        return nullptr;

    if (Frame* frame = downcast<Document>(*scriptExecutionContext()).frame()) {
        if (frame->isMainFrame())
            return nullptr;
    }

    return m_scriptExecutionContext->topOrigin();
}

void UserMediaPermissionCheck::contextDestroyed()
{
    ContextDestructionObserver::contextDestroyed();
}

void UserMediaPermissionCheck::start()
{
    ASSERT(scriptExecutionContext());

    auto& document = downcast<Document>(*scriptExecutionContext());
    UserMediaController* controller = UserMediaController::from(document.page());
    if (!controller)
        return;

    controller->checkUserMediaPermission(*this);
}

void UserMediaPermissionCheck::setUserMediaAccessInfo(const String& mediaDeviceIdentifierHashSalt, bool hasPersistentPermission)
{
    m_hasPersistentPermission = hasPersistentPermission;
    m_mediaDeviceIdentifierHashSalt = mediaDeviceIdentifierHashSalt;

    if (m_client)
        m_client->didCompletePermissionCheck(m_mediaDeviceIdentifierHashSalt, m_hasPersistentPermission);
}

} // namespace WebCore

#endif // ENABLE(MEDIA_STREAM)
