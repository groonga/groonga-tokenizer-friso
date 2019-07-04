#include <groonga/tokenizer.h>

#include <string.h>
#include <stdio.h>
#include "config.h"
#include "friso/friso.h"

#ifdef __GNUC__
#  define GNUC_UNUSED __attribute__((__unused__))
#else
#  define GNUC_UNUSED
#endif

typedef struct {
  friso_t friso;
  friso_config_t config;
  friso_task_t task;
  const char *next;
  const char *end;
  int pending;
  grn_tokenizer_query *query;
  grn_tokenizer_token token;
} friso_tokenizer;


static grn_obj *
friso_tokenizer_init(grn_ctx *ctx, int nargs, grn_obj **args, grn_user_data *user_data)
{
  friso_tokenizer *tokenizer;
  unsigned int normalizer_flags = 0;
  grn_tokenizer_query *query;
  grn_obj *normalized_query;
  const char *normalized_string;
  unsigned int normalized_string_length;

  query = grn_tokenizer_query_open(ctx, nargs, args, normalizer_flags);
  if (!query) {
    return NULL;
  }

  tokenizer = GRN_PLUGIN_MALLOC(ctx, sizeof(friso_tokenizer));
  if (!tokenizer) {
    grn_tokenizer_query_close(ctx, query);
    GRN_PLUGIN_ERROR(ctx, GRN_NO_MEMORY_AVAILABLE,
                     "[tokenizer][friso] "
                     "memory allocation to friso_tokenizer failed");
    return NULL;
  }

  tokenizer->query = query;

  normalized_query = query->normalized_query;
  grn_string_get_normalized(ctx,
                            normalized_query,
                            &normalized_string,
                            &normalized_string_length,
                            NULL);
  tokenizer->next = normalized_string;

  tokenizer->friso = friso_new();
  tokenizer->config = friso_new_config();
  tokenizer->pending = GRN_FALSE;
  
  if (!friso_init_from_ifile(tokenizer->friso,
                             tokenizer->config,
                             FRISO_INI_PATH)) {
    grn_tokenizer_query_close(ctx, query);
    GRN_PLUGIN_ERROR(ctx, GRN_NO_SUCH_FILE_OR_DIRECTORY,
                     "[tokenizer][friso] "
                     "friso initialization failed");
    return NULL;
  }

  user_data->ptr = tokenizer;

  if (normalized_string_length == 0) {
    tokenizer->next = "";
    tokenizer->end = tokenizer->next;
  } else {
    tokenizer->next = normalized_string;
    tokenizer->end = tokenizer->next + normalized_string_length;
  }

  grn_tokenizer_token_init(ctx, &(tokenizer->token));

  return NULL;
}

static void friso_token_push(grn_ctx *ctx,
                             friso_tokenizer *tokenizer)
{
  friso_t friso;
  friso_config_t config;
  friso_task_t task;
  grn_tokenizer_status status;
  const char *token;
  int token_length;

  friso = tokenizer->friso;
  config = tokenizer->config;
  task = tokenizer->task;
  token = tokenizer->next;
  if (next_mmseg_token(friso, config, task) != NULL) {
    token_length = task->token->length;
    tokenizer->next += token_length;

    if (tokenizer->next == tokenizer->end) {
      status = GRN_TOKENIZER_LAST;
    } else {
      status = GRN_TOKENIZER_CONTINUE;
    }
    grn_tokenizer_token_push(ctx, &(tokenizer->token),
                             token,
                             token_length, status);
  } else {
    if (tokenizer->pending) {
      tokenizer->pending = GRN_FALSE;
      friso_free_task(task);
    }
  }
}

static grn_obj *
friso_tokenizer_next(grn_ctx *ctx, GNUC_UNUSED int nargs, GNUC_UNUSED grn_obj **args,
                     grn_user_data *user_data)
{
  friso_tokenizer *tokenizer = user_data->ptr;
  grn_encoding encoding = tokenizer->query->encoding;
  const char *token;
  
  if (tokenizer->query->have_tokenized_delimiter) {
    tokenizer->next =
      grn_tokenizer_tokenized_delimiter_next(ctx,
                                             &(tokenizer->token),
                                             tokenizer->next,
                                             tokenizer->end - tokenizer->next,
                                             encoding);
  } else {

    token = tokenizer->next;
    if (tokenizer->pending) {
      friso_token_push(ctx, tokenizer);
    } else {
      
      tokenizer->task = friso_new_task();

      friso_set_text(tokenizer->task, (char *)token);
      tokenizer->pending = GRN_TRUE;

      friso_token_push(ctx, tokenizer);
    }
  }

  return NULL;
}

static grn_obj *
friso_tokenizer_fin(grn_ctx *ctx, GNUC_UNUSED int nargs, GNUC_UNUSED grn_obj **args,
                    grn_user_data *user_data)
{
  friso_tokenizer *tokenizer = user_data->ptr;

  if (!tokenizer) {
    return NULL;
  }

  friso_free_config(tokenizer->config);
  friso_free(tokenizer->friso);

  grn_tokenizer_token_fin(ctx, &(tokenizer->token));
  grn_tokenizer_query_close(ctx, tokenizer->query);
  GRN_PLUGIN_FREE(ctx, tokenizer);

  return NULL;
}


grn_rc
GRN_PLUGIN_INIT(grn_ctx *ctx)
{
  return ctx->rc;
}

grn_rc
GRN_PLUGIN_REGISTER(grn_ctx *ctx)
{
  grn_rc rc;

  rc = grn_tokenizer_register(ctx, "TokenFriso", -1,
                              friso_tokenizer_init,
                              friso_tokenizer_next,
                              friso_tokenizer_fin);

  return rc;
}

grn_rc
GRN_PLUGIN_FIN(GNUC_UNUSED grn_ctx *ctx)
{
  return GRN_SUCCESS;
}
