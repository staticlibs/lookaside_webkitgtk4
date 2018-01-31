// Copyright 2016 The Chromium Authors. All rights reserved.
// Copyright (C) 2016 Apple Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//    * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//    * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#ifndef CSSPropertyParserHelpers_h
#define CSSPropertyParserHelpers_h

// FIXME-NEWPARSER #include "CSSCustomIdentValue.h"
#include "CSSParserMode.h"
#include "CSSParserTokenRange.h"
#include "CSSPrimitiveValue.h"
#include "Length.h" // For ValueRange

namespace WebCore {

class CSSStringValue;
class CSSURIValue;
class CSSValuePair;

// When these functions are successful, they will consume all the relevant
// tokens from the range and also consume any whitespace which follows. When
// the start of the range doesn't match the type we're looking for, the range
// will not be modified.
namespace CSSPropertyParserHelpers {

// FIXME: These should probably just be consumeComma and consumeSlash.
bool consumeCommaIncludingWhitespace(CSSParserTokenRange&);
bool consumeSlashIncludingWhitespace(CSSParserTokenRange&);
// consumeFunction expects the range starts with a FunctionToken.
CSSParserTokenRange consumeFunction(CSSParserTokenRange&);

enum class UnitlessQuirk {
    Allow,
    Forbid
};

CSSPrimitiveValue* consumeInteger(CSSParserTokenRange&, double minimumValue = -std::numeric_limits<double>::max());
CSSPrimitiveValue* consumePositiveInteger(CSSParserTokenRange&);
bool consumeNumberRaw(CSSParserTokenRange&, double& result);
CSSPrimitiveValue* consumeNumber(CSSParserTokenRange&, ValueRange);
CSSPrimitiveValue* consumeLength(CSSParserTokenRange&, CSSParserMode, ValueRange, UnitlessQuirk = UnitlessQuirk::Forbid);
CSSPrimitiveValue* consumePercent(CSSParserTokenRange&, ValueRange);
CSSPrimitiveValue* consumeLengthOrPercent(CSSParserTokenRange&, CSSParserMode, ValueRange, UnitlessQuirk = UnitlessQuirk::Forbid);
CSSPrimitiveValue* consumeAngle(CSSParserTokenRange&);
CSSPrimitiveValue* consumeTime(CSSParserTokenRange&, ValueRange);

CSSPrimitiveValue* consumeIdent(CSSParserTokenRange&);
CSSPrimitiveValue* consumeIdentRange(CSSParserTokenRange&, CSSValueID lower, CSSValueID upper);
template<CSSValueID, CSSValueID...> inline bool identMatches(CSSValueID id);
template<CSSValueID... allowedIdents> CSSPrimitiveValue* consumeIdent(CSSParserTokenRange&);

// FIXME-NEWPARSER CSSCustomIdentValue* consumeCustomIdent(CSSParserTokenRange&);
CSSStringValue* consumeString(CSSParserTokenRange&);
StringView consumeUrlAsStringView(CSSParserTokenRange&);
CSSURIValue* consumeUrl(CSSParserTokenRange&);

CSSValue* consumeColor(CSSParserTokenRange&, CSSParserMode, bool acceptQuirkyColors = false);

CSSValuePair* consumePosition(CSSParserTokenRange&, CSSParserMode, UnitlessQuirk);
bool consumePosition(CSSParserTokenRange&, CSSParserMode, UnitlessQuirk, CSSValue*& resultX, CSSValue*& resultY);
bool consumeOneOrTwoValuedPosition(CSSParserTokenRange&, CSSParserMode, UnitlessQuirk, CSSValue*& resultX, CSSValue*& resultY);

enum class ConsumeGeneratedImage {
    Allow,
    Forbid
};

CSSValue* consumeImage(CSSParserTokenRange&, CSSParserContext, ConsumeGeneratedImage = ConsumeGeneratedImage::Allow);
CSSValue* consumeImageOrNone(CSSParserTokenRange&, CSSParserContext);

// Template implementations are at the bottom of the file for readability.

template<typename... emptyBaseCase> inline bool identMatches(CSSValueID) { return false; }
template<CSSValueID head, CSSValueID... tail> inline bool identMatches(CSSValueID id)
{
    return id == head || identMatches<tail...>(id);
}

// FIXME-NEWPARSER - converted to a RefPtr return type from a raw ptr.
template<CSSValueID... names> RefPtr<CSSPrimitiveValue> consumeIdent(CSSParserTokenRange& range)
{
    if (range.peek().type() != IdentToken || !identMatches<names...>(range.peek().id()))
        return nullptr;
    return CSSPrimitiveValue::createIdentifier(range.consumeIncludingWhitespace().id());
}

static inline bool isCSSWideKeyword(const CSSValueID& id)
{
    return id == CSSValueInitial || id == CSSValueInherit || id == CSSValueUnset || id == CSSValueDefault;
}

} // namespace CSSPropertyParserHelpers

} // namespace WebCore

#endif // CSSPropertyParserHelpers_h
