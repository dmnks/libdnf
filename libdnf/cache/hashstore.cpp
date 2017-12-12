#include <iostream>
#include <glib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "hashstore.hpp"

using namespace std;

bool file_exists(string filename)
{
    struct stat buf;
    return (stat(filename.c_str(), &buf) == 0);
}

void mkdirs(string path, mode_t mode)
{
    string subpath = "";
    string::iterator it;
    for (it = path.begin(); it < path.end(); it++) {
        if ((*it == '/') && (!subpath.empty()) && (!file_exists(subpath)))
            mkdir(subpath.c_str(), mode);
        subpath += *it;
    }
}

string get_csum(string filename, GChecksumType ctype)
{
    FILE *fp;
    if (!(fp = fopen(filename.c_str(), "rb")))
        return "";
    guchar buffer[4096];
    gssize len = 0;
    GChecksum *csum = g_checksum_new(ctype);
    while ((len = fread(buffer, 1, sizeof(buffer), fp)) > 0)
        g_checksum_update(csum, buffer, len);
    gchar *result = g_strdup(g_checksum_get_string(csum));
    g_checksum_free(csum);
    return string(result);
}

HashStore::HashStore(string root, GChecksumType ctype,
                     int depth, int width, mode_t mode) :
    root(root),
    ctype(ctype),
    depth(depth),
    width(width),
    mode(mode)
{}

string HashStore::get_path(string hash)
{
    string path = "";
    string::iterator it;
    int i = 0;
    int max = depth * width;
    for (it = hash.begin(); it < hash.end(); it++, i++) {
        if ((i > 0) && (i <= max) && (i % width == 0))
            path += '/';
        path += *it;
    }
    return path;
}

string HashStore::get(string hash)
{
    return root + "/" + get_path(hash);
}

string HashStore::put(string filename)
{
    string hash = get_csum(filename, ctype);
    string dest = get(hash);
    if (!file_exists(dest)) {
        mkdirs(dest, mode);
        link(filename.c_str(), dest.c_str());
    }
    return hash;
}

// TEST
int main(int argc, char **argv)
{
    HashStore hst = HashStore("/tmp/mystore");
    string hash = hst.put(argv[1]);
    string fname = hst.get(hash);
    cout << "Your inode is here: " << fname << endl;
}
