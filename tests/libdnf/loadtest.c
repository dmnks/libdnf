#include <glib.h>
#include <stdlib.h>
#include <stdio.h>
#include <librepo/librepo.h>
#include "libdnf/hy-repo.h"
#include "libdnf/hy-repo-private.hpp"

int main(void)
{
    HyRepo repo = hy_repo_create("test");
    HyRemote remote;
    HyMeta meta;

    remote.url = "http://download.fedoraproject.org/pub/fedora/linux/releases/27/Everything/x86_64/os/";
    remote.cachedir = "/var/tmp/loadtest";
    remote.maxage = 60;
    remote.gpgcheck = 0;
    remote.max_mirror_tries = 3;
    remote.max_parallel_downloads = 1;

    hy_repo_load(repo, &remote, &meta);
    printf("repomd_fn: %s\n", repo->repomd_fn);
    printf("primary_fn: %s\n", repo->primary_fn);
    printf("age: %i\n", meta.age);
    hy_repo_free(repo);

    return 0;
}