/*
 * Copyright (C) 2005-2012 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "Auth/SARC4.h"
#include <openssl/sha.h>

SARC4::SARC4(uint8 len) : m_ctx()
{
    #if OPENSSL_VERSION_NUMBER < 0x10100000L
      m_ctx = (EVP_CIPHER_CTX *) malloc(sizeof(EVP_CIPHER_CTX));
      EVP_CIPHER_CTX_init(m_ctx);
    #else
      m_ctx = EVP_CIPHER_CTX_new();
    #endif
    EVP_EncryptInit_ex(m_ctx, EVP_rc4(), NULL, NULL, NULL);
    EVP_CIPHER_CTX_set_key_length(m_ctx, len);
}

SARC4::SARC4(uint8 *seed, uint8 len) : m_ctx()
{
    #if OPENSSL_VERSION_NUMBER < 0x10100000L
      m_ctx = (EVP_CIPHER_CTX *) malloc(sizeof(EVP_CIPHER_CTX));
      EVP_CIPHER_CTX_init(m_ctx);
    #else
      m_ctx = EVP_CIPHER_CTX_new();
    #endif
    EVP_EncryptInit_ex(m_ctx, EVP_rc4(), NULL, NULL, NULL);
    EVP_CIPHER_CTX_set_key_length(m_ctx, len);
    EVP_EncryptInit_ex(m_ctx, NULL, NULL, seed, NULL);
}

SARC4::~SARC4()
{
    #if OPENSSL_VERSION_NUMBER < 0x10100000L
      EVP_CIPHER_CTX_cleanup(m_ctx);
      free(m_ctx);
    #else
      EVP_CIPHER_CTX_free(m_ctx);
    #endif
}

void SARC4::Init(uint8 *seed)
{
    EVP_EncryptInit_ex(m_ctx, NULL, NULL, seed, NULL);
}

void SARC4::UpdateData(int len, uint8 *data)
{
    int outlen = 0;
    EVP_EncryptUpdate(m_ctx, data, &outlen, data, len);
    EVP_EncryptFinal_ex(m_ctx, data, &outlen);
}
