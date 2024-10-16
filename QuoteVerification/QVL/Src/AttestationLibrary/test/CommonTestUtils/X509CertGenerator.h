/*
 * Copyright (C) 2011-2020 Intel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Intel Corporation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
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
 *
 */

#ifndef SGXECDSAATTESTATION_X509CERTGENERATOR_H
#define SGXECDSAATTESTATION_X509CERTGENERATOR_H

#include <stdio.h>
#include <stdlib.h>

#include <SgxEcdsaAttestation/AttestationParsers.h>

#include <openssl/pem.h>
#include <openssl/conf.h>
#include <openssl/x509v3.h>
#include <openssl/asn1t.h>

#include "PckParser/PckParser.h"
#include "X509CertTypes.h"
#include "OpensslHelpers/Bytes.h"
#include "OpensslHelpers/OpensslTypes.h"

namespace intel { namespace sgx { namespace qvl { namespace test {

class X509CertGenerator {
public:
    crypto::X509_uptr generateCaCert(int version, const Bytes &serialNumber, long notBeforeOffset, long notAfterOffset,
                                     const EVP_PKEY *pubKey, const EVP_PKEY *signingKey,
                                     const dcap::parser::x509::DistinguishedName &subject, const dcap::parser::x509::DistinguishedName &issuer) const;

    crypto::X509_uptr generateTcbSigningCert(int version, const Bytes &serialNumber, long notBeforeOffset, long notAfterOffset,
                                             const EVP_PKEY *pubKey, const EVP_PKEY *signingKey,
                                             const dcap::parser::x509::DistinguishedName &subject, const dcap::parser::x509::DistinguishedName &issuer) const;

    crypto::X509_uptr generatePCKCert(int version, const Bytes &serialNumber, long notBeforeOffset, long notAfterOffset,
                                   const EVP_PKEY *pubKey, const EVP_PKEY *signingKey,
                                   const dcap::parser::x509::DistinguishedName &subject,
                                   const dcap::parser::x509::DistinguishedName &issuer,
                                   const Bytes& ppid, const Bytes& cpusvn, const Bytes& pcesvn,
                                   const Bytes& pceId, const Bytes& fmspc,
                                   bool dynamicPlatform = false, bool includeDynamicPlatform = false) const;

    crypto::EVP_PKEY_uptr generateEcKeypair() const;

    std::string x509ToString(const X509 *cert);

private:
    crypto::X509_uptr generateBaseCert(int version, const Bytes &serialNumber,
        long notBeforeOffset, long notAfterOffset,
        const EVP_PKEY *pubKey,
        const dcap::parser::x509::DistinguishedName &subject, const dcap::parser::x509::DistinguishedName &issuer) const;

    void addStandardCaExtensions(const crypto::X509_uptr& cert) const;
    void addStandardNonCaExtensions(const crypto::X509_uptr& cert) const;
    void addSGXPckExtensions(const crypto::X509_uptr &cert, const Bytes &ppid, const Bytes &cpusvn, const Bytes &pcesvn,
                             const Bytes &pceId, const Bytes &fmspc,
                             bool dynamicPlatform = false, bool includeDynamicPlatform = false) const;

    void add_ext(X509 *cert, int nid, char *value) const;
    void createTcbComp(const std::string &parentOid, int index, unsigned char value, crypto::SGX_INT* dest) const;

    void signCert(const crypto::X509_uptr& cert, const EVP_PKEY *signingKey) const;
};

}}}}

#endif //SGXECDSAATTESTATION_X509CERTGENERATOR_H
