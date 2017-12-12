#ifndef LIBDNF_HASHSTORE_H
#define LIBDNF_HASHSTORE_H

#include <string>
#include <glib.h>
#include <sys/types.h>

/// Simple content-addressable filesystem
/**
 * Implements a hash table that maps file checksums to inodes using a directory
 * tree where filenames are the checksums.
 *
 * No files are ever copied; the system only references existing inodes.
 */
class HashStore
{
public:
    /// Constructs a HashStore object
    /**
     * @param root  Base directory for this filesystem
     * @param ctype Hash algorithm to use
     * @param depth Number of directory levels to use to partition the tree
     * @param width Number of chars in a hash to use per directory level
     * @param mode  Mode to use when creating directories
     */
    HashStore(std::string root, GChecksumType ctype = G_CHECKSUM_SHA256,
              int depth = 1, int width = 2, mode_t mode = 0755);
    /// Inserts a file into the system
    /**
     * @return Hash of the file that was inserted
     */
    std::string put(std::string filename);
    /// Translates a hash to a filename
    /*
     * @return Absolute path to the file stored under the given hash
     */
    std::string get(std::string hash);

private:
    std::string root;
    GChecksumType ctype;
    int depth;
    int width;
    mode_t mode;
    std::string get_path(std::string hash);
};

#endif /* LIBDNF_HASHSTORE_H */
