<?php

/**
 * @file
 * Functions to handle the googledrive backup destination.
 */

/**
 * A destination for sending backups to a googledrive account.
 *
 * @ingroup backup_migrate_destinations
 */
class backup_migrate_destination_googledrive extends backup_migrate_destination_remote {
  var $supported_ops = array('scheduled backup', 'manual backup');
  var $dropbox = NULL;

  /**
   * Save to to the Google Drive destination.
   */
  function save_file($file, $settings) {
    $dropbox = $this->dropbox_object();
    $destination = $this->dest_url['path'];
    if ('/' != $destination[0]) {
      $destination = '/'. $destination;
    }

    $dest_filename = realpath(variable_get('file_temporary_path', '')) . '/' . $file->file_info['filename'] . '.' . implode('.', $file->ext);

    rename($file->filepath(), $dest_filename);
    try {
      $dropbox->upload($dest_filename, $destination);
    }
    catch (Exception $e) {
      watchdog('backup_migrate',
               'There was a problem when we tried to save the file to Google Drive, the error message was: !error',
               array('!error' => $e->getMessage()),
               WATCHDOG_ERROR);
      return FALSE;
    }
    return $file;
  }

  /**
   * Get the form for the settings for this filter.
   */
  function edit_form() {
    $form = parent::edit_form();
    $form['scheme']['#type'] = 'value';
    $form['scheme']['#value'] = 'https';
    $form['host']['#type'] = 'value';
    $form['host']['#value'] = 'www.dropbox.com';
    $form['path']['#description'] = 'A relative folder inside your Google Drive account';
    $form['user']['#title'] = 'Google E-mail';
    $form['pass']['#title'] = 'Google Password';

    return $form;
  }

}
