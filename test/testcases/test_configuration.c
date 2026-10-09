/*
 * Copyright (C) 2026 The pgvictoria community
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list
 * of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or other
 * materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may
 * be used to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <pgvictoria.h>
#include <configuration.h>
#include <mctf.h>
#include <tscommon.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

MCTF_TEST(test_configuration_indented_lines)
{
   char* path = "test_configuration_indented.conf";
   struct main_configuration* config = NULL;
   FILE* f = NULL;

   config = (struct main_configuration*)calloc(1, sizeof(struct main_configuration));
   MCTF_ASSERT_PTR_NONNULL(config, cleanup, "could not allocate the configuration");
   pgvictoria_init_main_configuration(config);

   f = fopen(path, "w");
   MCTF_ASSERT_PTR_NONNULL(f, cleanup, "could not create %s", path);
   fprintf(f, "[pgvictoria]\n");
   fprintf(f, "            host = localhost\n");
   fprintf(f, "   unix_socket_dir = /tmp/   # comment\n");
   fclose(f);
   f = NULL;

   MCTF_ASSERT_INT_EQ(pgvictoria_read_main_configuration(config, path), 0, cleanup, "reading the configuration failed");
   MCTF_ASSERT_STR_EQ(config->host, "localhost", cleanup, "host mismatch");
   MCTF_ASSERT_STR_EQ(config->unix_socket_dir, "/tmp/", cleanup, "unix_socket_dir mismatch");

cleanup:
   if (f != NULL)
   {
      fclose(f);
   }
   unlink(path);
   free(config);
   MCTF_FINISH();
}
