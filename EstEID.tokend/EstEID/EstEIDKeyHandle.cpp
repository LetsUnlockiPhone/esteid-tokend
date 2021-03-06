/*
 * EstEID.tokend
 *
 * This software is released under either the GNU Library General Public
 * License (see LICENSE.LGPL).
 *
 * Note that the only valid version of the LGPL license as far as this
 * project is concerned is the original GNU Library General Public License
 * Version 2.1, February 1999
 *
 */

#include "EstEIDKeyHandle.h"

#include "EstEIDRecord.h"
#include "EstEIDToken.h"

#include "EstEID_utility.h"

//
// EstEIDKeyHandle
//
EstEIDKeyHandle::EstEIDKeyHandle(EstEIDToken &token,
                                 const Tokend::MetaRecord &metaRecord,
                                 Tokend::Record &record) :
Tokend::KeyHandle(metaRecord, &record),
mToken(token) {
    FLOG;
}

EstEIDKeyHandle::~EstEIDKeyHandle() {
}

void EstEIDKeyHandle::getKeySize(CSSM_KEY_SIZE &keySize) {
    FLOG;
    CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}

uint32 EstEIDKeyHandle::getOutputSize(const Context &context,
                                      uint32 inputSize, bool encrypting) {
    FLOG;
    CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}

void EstEIDKeyHandle::generateSignature(const Context &context,
                                        CSSM_ALGORITHMS signOnly, const CssmData &input, CssmData &signature) {
    FLOG;
    _log("EstEIDKeyHandle::generateSignature alg: %u signOnly: %X", context.algorithm(), signOnly);
    IFDUMPING("esteid.tokend", context.dump("signature context"));
    
    if (context.type() != CSSM_ALGCLASS_SIGNATURE)
        CssmError::throwMe(CSSMERR_CSP_INVALID_CONTEXT);
    
    if (context.algorithm() != CSSM_ALGID_RSA)
        CssmError::throwMe(CSSMERR_CSP_INVALID_ALGORITHM);
    
    // The format of the input. SSL means "raw, no hash OID"
    // smartcardpp adds the necessary prefixes.
    EstEIDManager::AlgType type = EstEIDManager::SSL;

    if (signOnly == CSSM_ALGID_NONE) {
        _log("SSL signature request");
    } else if (signOnly == CSSM_ALGID_SHA256) {
        type = EstEIDManager::SHA256;
    } else if (signOnly == CSSM_ALGID_SHA384) {
        type = EstEIDManager::SHA384;
    } else if (signOnly == CSSM_ALGID_SHA512) {
        type = EstEIDManager::SHA512;
    } else {
        CssmError::throwMe(CSSMERR_CSP_INVALID_DIGEST_ALGORITHM);
    }
#if !defined(NDEBUG)
    context.dump("signature context");
#endif
    
    uint32 padding = CSSM_PADDING_PKCS1;
    context.getInt(CSSM_ATTRIBUTE_PADDING, padding);
    
    if (padding != CSSM_PADDING_PKCS1)
        CssmError::throwMe(CSSMERR_CSP_INVALID_ATTR_PADDING);
    
    try {
        ByteVec result = mToken.getCard().sign(ByteVec(input.Data, input.Data + input.Length), type, EstEIDManager::AUTH, mToken.getPIN1());
        unsigned char *outputData = reinterpret_cast<unsigned char *>(malloc(result.size()));
        memcpy(outputData, &result[0], result.size());
        signature.Data = outputData;
        signature.Length = result.size();
    } catch(std::runtime_error &err) {
        _log("exception while signing");
        CssmError::throwMe(CSSMERR_CSP_FUNCTION_FAILED);
    }
}

void EstEIDKeyHandle::verifySignature(const Context &context,
                                      CSSM_ALGORITHMS signOnly, const CssmData &input, const CssmData &signature) {
    FLOG;
    CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}

void EstEIDKeyHandle::generateMac(const Context &context,
                                  const CssmData &input, CssmData &output) {
    FLOG;
    CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}

void EstEIDKeyHandle::verifyMac(const Context &context,
                                const CssmData &input, const CssmData &compare) {
    FLOG;
    CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}

void EstEIDKeyHandle::encrypt(const Context &context,
                              const CssmData &clear, CssmData &cipher) {
    FLOG;
    CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}

void EstEIDKeyHandle::decrypt(const Context &context,
                              const CssmData &cipher, CssmData &clear) {
    FLOG;
    CssmError::throwMe(CSSMERR_CSP_KEY_USAGE_INCORRECT);
    // FIXME: this is missing.
}

void EstEIDKeyHandle::exportKey(const Context &context,
                                const AccessCredentials *cred, CssmKey &wrappedKey) {
    FLOG;
    CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}


//
// EstEIDKeyHandleFactory
//
EstEIDKeyHandleFactory::~EstEIDKeyHandleFactory() {
}


Tokend::KeyHandle *EstEIDKeyHandleFactory::keyHandle(
                                                     Tokend::TokenContext *tokenContext, const Tokend::MetaRecord &metaRecord,
                                                     Tokend::Record &record) const {
    FLOG;
    //	EstEIDKeyRecord &keyRecord = dynamic_cast<EstEIDKeyRecord &>(record);
    EstEIDToken &eToken = static_cast<EstEIDToken &>(*tokenContext);
    return new EstEIDKeyHandle(eToken, metaRecord, record);
}

