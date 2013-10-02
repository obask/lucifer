#ifndef Py_SPAMMODULE_H
#define Py_SPAMMODULE_H
#ifdef __cplusplus
extern "C" {
#endif


int
apiSaveIndex(char *path);

int
apiLoadIndex(char *path);

int
apiAddDocument(char *_id, char *data);

const char *
apiSearchQuery(char *data, int f1, int f2);

int
apiShowIndex(char *data);


extern char *
tolower_stem_porter(char * b);


#ifdef __cplusplus
}
#endif

#endif /* !defined(Py_SPAMMODULE_H) */

