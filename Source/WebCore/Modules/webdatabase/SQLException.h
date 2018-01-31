/*
 * Copyright (C) 2010 Google Inc. All rights reserved.
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

#ifndef SQLException_h
#define SQLException_h

#include "ExceptionBase.h"

namespace WebCore {

class SQLException : public ExceptionBase {
public:
    static Ref<SQLException> create(const ExceptionCodeDescription& description)
    {
        return adoptRef(*new SQLException(description));
    }

    static const int SQLExceptionOffset = 1000;
    static const int SQLExceptionMax = 1099;

    enum SQLExceptionCode {
        UNKNOWN_ERR = SQLExceptionOffset,
        DATABASE_ERR = SQLExceptionOffset + 1,
        VERSION_ERR = SQLExceptionOffset + 2,
        TOO_LARGE_ERR = SQLExceptionOffset + 3,
        QUOTA_ERR = SQLExceptionOffset + 4,
        SYNTAX_ERR = SQLExceptionOffset + 5,
        CONSTRAINT_ERR = SQLExceptionOffset + 6,
        TIMEOUT_ERR = SQLExceptionOffset + 7
    };

    static bool initializeDescription(ExceptionCode, ExceptionCodeDescription*);

private:
    explicit SQLException(const ExceptionCodeDescription& description)
        : ExceptionBase(description)
    {
    }
};

} // namespace WebCore

#endif // SQLException_h
