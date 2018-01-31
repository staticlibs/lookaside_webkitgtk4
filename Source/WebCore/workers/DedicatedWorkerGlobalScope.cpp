/*
 * Copyright (C) 2009 Google Inc. All rights reserved.
 * Copyright (C) 2016 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "DedicatedWorkerGlobalScope.h"

#include "ContentSecurityPolicyResponseHeaders.h"
#include "DOMWindow.h"
#include "DedicatedWorkerThread.h"
#include "MessageEvent.h"
#include "SecurityOrigin.h"
#include "WorkerObjectProxy.h"

namespace WebCore {

Ref<DedicatedWorkerGlobalScope> DedicatedWorkerGlobalScope::create(const URL& url, const String& userAgent, DedicatedWorkerThread& thread, const ContentSecurityPolicyResponseHeaders& contentSecurityPolicyResponseHeaders, bool shouldBypassMainWorldContentSecurityPolicy, PassRefPtr<SecurityOrigin> topOrigin, IDBClient::IDBConnectionProxy* connectionProxy, SocketProvider* socketProvider)
{
    Ref<DedicatedWorkerGlobalScope> context = adoptRef(*new DedicatedWorkerGlobalScope(url, userAgent, thread, shouldBypassMainWorldContentSecurityPolicy, topOrigin, connectionProxy, socketProvider));
    if (!shouldBypassMainWorldContentSecurityPolicy)
        context->applyContentSecurityPolicyResponseHeaders(contentSecurityPolicyResponseHeaders);
    return context;
}

DedicatedWorkerGlobalScope::DedicatedWorkerGlobalScope(const URL& url, const String& userAgent, DedicatedWorkerThread& thread, bool shouldBypassMainWorldContentSecurityPolicy, PassRefPtr<SecurityOrigin> topOrigin, IDBClient::IDBConnectionProxy* connectionProxy, SocketProvider* socketProvider)
    : WorkerGlobalScope(url, userAgent, thread, shouldBypassMainWorldContentSecurityPolicy, topOrigin, connectionProxy, socketProvider)
{
}

DedicatedWorkerGlobalScope::~DedicatedWorkerGlobalScope()
{
}

EventTargetInterface DedicatedWorkerGlobalScope::eventTargetInterface() const
{
    return DedicatedWorkerGlobalScopeEventTargetInterfaceType;
}

void DedicatedWorkerGlobalScope::postMessage(RefPtr<SerializedScriptValue>&& message, const MessagePortArray* ports, ExceptionCode& ec)
{
    // Disentangle the port in preparation for sending it to the remote context.
    auto channels = MessagePort::disentanglePorts(ports, ec);
    if (ec)
        return;
    thread().workerObjectProxy().postMessageToWorkerObject(WTFMove(message), WTFMove(channels));
}

void DedicatedWorkerGlobalScope::importScripts(const Vector<String>& urls, ExceptionCode& ec)
{
    Base::importScripts(urls, ec);
    thread().workerObjectProxy().reportPendingActivity(hasPendingActivity());
}

DedicatedWorkerThread& DedicatedWorkerGlobalScope::thread()
{
    return static_cast<DedicatedWorkerThread&>(Base::thread());
}

} // namespace WebCore
