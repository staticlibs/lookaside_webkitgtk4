/*
 * Copyright (C) 2009 Apple Inc. All Rights Reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
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
 */

#ifndef CredentialStorage_h
#define CredentialStorage_h

#include "Credential.h"
#include "ProtectionSpaceHash.h"
#include <wtf/HashMap.h>
#include <wtf/HashSet.h>
#include <wtf/text/StringHash.h>
#include <wtf/text/WTFString.h>

namespace WebCore {

class URL;
class ProtectionSpace;

class CredentialStorage {
public:
    WEBCORE_EXPORT static CredentialStorage& defaultCredentialStorage();

    // WebCore session credential storage.
    WEBCORE_EXPORT void set(const Credential&, const ProtectionSpace&, const URL&);
    WEBCORE_EXPORT Credential get(const ProtectionSpace&);
    WEBCORE_EXPORT void remove(const ProtectionSpace&);

    // OS persistent storage.
    WEBCORE_EXPORT Credential getFromPersistentStorage(const ProtectionSpace&);

    WEBCORE_EXPORT void clearCredentials();

#if PLATFORM(IOS)
    void saveToPersistentStorage(const ProtectionSpace&, const Credential&);
#endif

    // These methods work for authentication schemes that support sending credentials without waiting for a request. E.g., for HTTP Basic authentication scheme
    // a client should assume that all paths at or deeper than the depth of a known protected resource share are within the same protection space.
    WEBCORE_EXPORT bool set(const Credential&, const URL&); // Returns true if the URL corresponds to a known protection space, so credentials could be updated.
    WEBCORE_EXPORT Credential get(const URL&);

private:
    HashMap<ProtectionSpace, Credential> m_protectionSpaceToCredentialMap;
    HashSet<String> m_originsWithCredentials;

    typedef HashMap<String, ProtectionSpace> PathToDefaultProtectionSpaceMap;
    PathToDefaultProtectionSpaceMap m_pathToDefaultProtectionSpaceMap;

    PathToDefaultProtectionSpaceMap::iterator findDefaultProtectionSpaceForURL(const URL&);
};

} // namespace WebCore

#endif // CredentialStorage_h
