/* GLib testing framework examples and tests
 *
 * Copyright (C) 2011 Collabora Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author: Nicolas Dufresne <nicolas.dufresne@collabora.com>
 */

#include "config.h"

#include <gio/gio.h>

#include "gtesttlsbackend.h"

typedef struct
{
  gchar *cert_pems[3];
  gchar *key_pem;
  gchar *key8_pem;
} Reference;

static void
pem_parser (const Reference *ref)
{
  GTlsCertificate *cert;
  gchar *pem;
  gchar *parsed_cert_pem = NULL;
  const gchar *parsed_key_pem = NULL;
  GError *error = NULL;

  /* Check PEM parsing in certificate, private key order. */
  g_file_get_contents (SRCDIR "/cert-key.pem", &pem, NULL, &error);
  g_assert_no_error (error);
  g_assert (pem);

  cert = g_tls_certificate_new_from_pem (pem, -1, &error);
  g_assert_no_error (error);
  g_assert (cert);

  g_object_get (cert,
      "certificate-pem", &parsed_cert_pem,
      NULL);
  parsed_key_pem = g_test_tls_connection_get_private_key_pem (cert);
  g_assert_cmpstr (parsed_cert_pem, ==, ref->cert_pems[0]);
  g_free (parsed_cert_pem);
  parsed_cert_pem = NULL;
  g_assert_cmpstr (parsed_key_pem, ==, ref->key_pem);
  parsed_key_pem = NULL;

  g_object_unref (cert);

  /* Make sure length is respected and parser detect invalid (truncated) PEM. */
  cert = g_tls_certificate_new_from_pem (pem, 10, &error);
  g_assert_error (error, G_TLS_ERROR, G_TLS_ERROR_BAD_CERTIFICATE);
  g_clear_error (&error);
  g_free (pem);

  /* Check PEM parsing in private key, certificate order */
  g_file_get_contents (SRCDIR "/key-cert.pem", &pem, NULL, &error);
  g_assert_no_error (error);
  g_assert (pem);

  cert = g_tls_certificate_new_from_pem (pem, -1, &error);
  g_assert_no_error (error);
  g_assert (cert);

  g_object_get (cert,
      "certificate-pem", &parsed_cert_pem,
      NULL);
  parsed_key_pem = g_test_tls_connection_get_private_key_pem (cert);
  g_assert_cmpstr (parsed_cert_pem, ==, ref->cert_pems[0]);
  g_free (parsed_cert_pem);
  parsed_cert_pem = NULL;
  g_assert_cmpstr (parsed_key_pem, ==, ref->key_pem);
  parsed_key_pem = NULL;

  g_free (pem);
  g_object_unref (cert);

  /* Check certificate only PEM */
  g_file_get_contents (SRCDIR "/cert1.pem", &pem, NULL, &error);
  g_assert_no_error (error);
  g_assert (pem);

  cert = g_tls_certificate_new_from_pem (pem, -1, &error);
  g_assert_no_error (error);
  g_assert (cert);

  g_object_get (cert,
      "certificate-pem", &parsed_cert_pem,
      NULL);
  parsed_key_pem = g_test_tls_connection_get_private_key_pem (cert);
  g_assert_cmpstr (parsed_cert_pem, ==, ref->cert_pems[0]);
  g_free (parsed_cert_pem);
  parsed_cert_pem = NULL;
  g_assert (parsed_key_pem == NULL);

  g_free (pem);
  g_object_unref (cert);

  /* Check error with private key only PEM */
  g_file_get_contents (SRCDIR "/key.pem", &pem, NULL, &error);
  g_assert_no_error (error);
  g_assert (pem);

  cert = g_tls_certificate_new_from_pem (pem, -1, &error);
  g_assert_error (error, G_TLS_ERROR, G_TLS_ERROR_BAD_CERTIFICATE);
  g_clear_error (&error);
  g_assert (cert == NULL);
  g_free (pem);
}

static void
from_file (const Reference *ref)
{
  GTlsCertificate *cert;
  gchar *parsed_cert_pem = NULL;
  const gchar *parsed_key_pem = NULL;
  GError *error = NULL;

  cert = g_tls_certificate_new_from_file (SRCDIR "/key-cert.pem", &error);
  g_assert_no_error (error);
  g_assert (cert);

  g_object_get (cert,
      "certificate-pem", &parsed_cert_pem,
      NULL);
  parsed_key_pem = g_test_tls_connection_get_private_key_pem (cert);
  g_assert_cmpstr (parsed_cert_pem, ==, ref->cert_pems[0]);
  g_free (parsed_cert_pem);
  parsed_cert_pem = NULL;
  g_assert_cmpstr (parsed_key_pem, ==, ref->key_pem);
  parsed_key_pem = NULL;

  g_object_unref (cert);
}

static void
from_files (const Reference *ref)
{
  GTlsCertificate *cert;
  gchar *parsed_cert_pem = NULL;
  const gchar *parsed_key_pem = NULL;
  GError *error = NULL;

  cert = g_tls_certificate_new_from_files (SRCDIR "/cert1.pem",
                                           SRCDIR "/key.pem",
                                           &error);
  g_assert_no_error (error);
  g_assert (cert);

  g_object_get (cert,
      "certificate-pem", &parsed_cert_pem,
      NULL);
  parsed_key_pem = g_test_tls_connection_get_private_key_pem (cert);
  g_assert_cmpstr (parsed_cert_pem, ==, ref->cert_pems[0]);
  g_free (parsed_cert_pem);
  parsed_cert_pem = NULL;
  g_assert_cmpstr (parsed_key_pem, ==, ref->key_pem);
  parsed_key_pem = NULL;

  g_object_unref (cert);

  /* Missing private key */
  cert = g_tls_certificate_new_from_files (SRCDIR "/cert1.pem",
                                           SRCDIR "/cert2.pem",
                                           &error);
  g_assert_error (error, G_TLS_ERROR, G_TLS_ERROR_BAD_CERTIFICATE);
  g_clear_error (&error);
  g_assert (cert == NULL);

  /* Missing certificate */
  cert = g_tls_certificate_new_from_files (SRCDIR "/key.pem",
                                           SRCDIR "/key.pem",
                                           &error);
  g_assert_error (error, G_TLS_ERROR, G_TLS_ERROR_BAD_CERTIFICATE);
  g_clear_error (&error);
  g_assert (cert == NULL);

  /* Using this method twice with a file containing both private key and
   * certificate as a way to inforce private key presence is a fair use */
  cert = g_tls_certificate_new_from_files (SRCDIR "/key-cert.pem",
                                           SRCDIR "/key-cert.pem",
                                           &error);
  g_assert_no_error (error);
  g_assert (cert);
  g_object_unref (cert);
}


static void
from_files_pkcs8 (const Reference *ref)
{
  GTlsCertificate *cert;
  gchar *parsed_cert_pem = NULL;
  const gchar *parsed_key_pem = NULL;
  GError *error = NULL;

  cert = g_tls_certificate_new_from_files (SRCDIR "/cert1.pem",
                                           SRCDIR "/key8.pem",
                                           &error);
  g_assert_no_error (error);
  g_assert (cert);

  g_object_get (cert,
      "certificate-pem", &parsed_cert_pem,
      NULL);
  parsed_key_pem = g_test_tls_connection_get_private_key_pem (cert);
  g_assert_cmpstr (parsed_cert_pem, ==, ref->cert_pems[0]);
  g_free (parsed_cert_pem);
  parsed_cert_pem = NULL;
  g_assert_cmpstr (parsed_key_pem, ==, ref->key8_pem);
  parsed_key_pem = NULL;

  g_object_unref (cert);
}

static void
list_from_file (const Reference *ref)
{
  GList *list, *l;
  GError *error = NULL;
  int i;

  list = g_tls_certificate_list_new_from_file (SRCDIR "/cert-list.pem", &error);
  g_assert_no_error (error);
  g_assert_cmpint (g_list_length (list), ==, 3);

  l = list;
  for (i = 0; i < 3; i++)
    {
      GTlsCertificate *cert = l->data;
      gchar *parsed_cert_pem = NULL;
      g_object_get (cert,
          "certificate-pem", &parsed_cert_pem,
          NULL);
      g_assert_cmpstr (parsed_cert_pem, ==, ref->cert_pems[i]);
      g_free (parsed_cert_pem);
      l = g_list_next (l);
    }

  g_list_free_full (list, g_object_unref);

  /* Empty list is not an error */
  list = g_tls_certificate_list_new_from_file (SRCDIR "/nothing.pem", &error);
  g_assert_no_error (error);
  g_assert_cmpint (g_list_length (list), ==, 0);
}

int
main (int   argc,
      char *argv[])
{
  int rtv;
  Reference ref;
  GError *error = NULL;

  g_test_init (&argc, &argv, NULL);

  _g_test_tls_backend_get_type ();

  /* Load reference PEM */
  g_file_get_contents (SRCDIR "/cert1.pem", &ref.cert_pems[0], NULL, &error);
  g_assert_no_error (error);
  g_assert (ref.cert_pems[0]);
  g_file_get_contents (SRCDIR "/cert2.pem", &ref.cert_pems[1], NULL, &error);
  g_assert_no_error (error);
  g_assert (ref.cert_pems[1]);
  g_file_get_contents (SRCDIR "/cert3.pem", &ref.cert_pems[2], NULL, &error);
  g_assert_no_error (error);
  g_assert (ref.cert_pems[2]);
  g_file_get_contents (SRCDIR "/key.pem", &ref.key_pem, NULL, &error);
  g_assert_no_error (error);
  g_assert (ref.key_pem);
  g_file_get_contents (SRCDIR "/key8.pem", &ref.key8_pem, NULL, &error);
  g_assert_no_error (error);
  g_assert (ref.key8_pem);

  g_test_add_data_func ("/tls-certificate/pem-parser",
                        &ref, (GTestDataFunc)pem_parser);
  g_test_add_data_func ("/tls-certificate/from_file",
                        &ref, (GTestDataFunc)from_file);
  g_test_add_data_func ("/tls-certificate/from_files",
                        &ref, (GTestDataFunc)from_files);
  g_test_add_data_func ("/tls-certificate/from_files_pkcs8",
                        &ref, (GTestDataFunc)from_files_pkcs8);
  g_test_add_data_func ("/tls-certificate/list_from_file",
                        &ref, (GTestDataFunc)list_from_file);

  rtv = g_test_run();

  g_free (ref.cert_pems[0]);
  g_free (ref.cert_pems[1]);
  g_free (ref.cert_pems[2]);
  g_free (ref.key_pem);
  g_free (ref.key8_pem);

  return rtv;
}
