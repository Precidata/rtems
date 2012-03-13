/**
 * @file
 *
 * @ingroup LibFSIMFS
 *
 * @brief Mini-IMFS initialization.
 */

/*
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#if HAVE_CONFIG_H
  #include "config.h"
#endif

#include "imfs.h"

static const rtems_filesystem_operations_table miniIMFS_ops = {
  .lock_h = rtems_filesystem_default_lock,
  .unlock_h = rtems_filesystem_default_unlock,
  .eval_path_h = IMFS_eval_path,
  .link_h = rtems_filesystem_default_link,
  .are_nodes_equal_h = rtems_filesystem_default_are_nodes_equal,
  .node_type_h = IMFS_node_type,
  .mknod_h = IMFS_mknod,
  .rmnod_h = IMFS_rmnod,
  .fchmod_h = rtems_filesystem_default_fchmod,
  .chown_h = rtems_filesystem_default_chown,
  .clonenod_h = rtems_filesystem_default_clonenode,
  .freenod_h = rtems_filesystem_default_freenode,
  .mount_h = IMFS_mount,
  .fsmount_me_h = miniIMFS_initialize,
  .unmount_h = rtems_filesystem_default_unmount,
  .fsunmount_me_h = rtems_filesystem_default_fsunmount,
  .utime_h = rtems_filesystem_default_utime,
  .symlink_h = rtems_filesystem_default_symlink,
  .readlink_h = rtems_filesystem_default_readlink,
  .rename_h = rtems_filesystem_default_rename,
  .statvfs_h = rtems_filesystem_default_statvfs
};

int miniIMFS_initialize(
  rtems_filesystem_mount_table_entry_t *mt_entry,
  const void                           *data
)
{
  return IMFS_initialize_support(
    mt_entry,
    &miniIMFS_ops,
    &rtems_filesystem_handlers_default, /* for links */
    &rtems_filesystem_handlers_default  /* for fifos */
  );
}
