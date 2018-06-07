# Copyright 2012 (c) Kyan <kyan.ql.he@gmail.com>
{
  'variables': {
    'use_system_minizip%': 0,
    'use_system_zlib': 0,
    'os_bsd%': 0,
  },

  'targets': [
    {
      'target_name': 'zlib',
      'type': 'static_library',
      'conditions': [
        ['use_system_zlib == 0', {
          'sources': [
            'adler32.c',
            'compress.c',
            'crc32.c',
            'crc32.h',
            'deflate.c',
            'deflate.h',
            'gzio.c',
            'infback.c',
            'inffast.c',
            'inffast.h',
            'inffixed.h',
            'inflate.c',
            'inflate.h',
            'inftrees.c',
            'inftrees.h',
            'mozzconf.h',
            'trees.c',
            'trees.h',
            'uncompr.c',
            'zconf.h',
            'zlib.h',
            'zutil.c',
            'zutil.h',
          ], # sources

          'include_dirs': [
            '.',
          ],
          'direct_dependent_settings': {
            'include_dirs': [
              '.',
            ],
          },
        }, { # else, use_system_zlib == 1
          'direct_dependent_settings': {
            'defines': [
              'USE_SYSTEM_ZLIB',
            ],
          },
          'conditions': [
            ['OS == "ios"', {
              'link_settings': {
                'libraries': [
                  '$(SDKROOT)/usr/lib/libz.dylib',
                ],
              }
            }, { # else, OS != 'ios'
              'link_settings': {
                'libraries': [
                  '-lz',
                ],
              }
            }], # condition #1, OS == 'ios'

            ['OS == "android"', {
              # TODO(kyan): gyp does not support none type for Android NDK.
              'type': 'static_library',

              # TODO(kyan): android need LD_FLAGS set with '-lz'.
              'link_settings': {
                'ldflags': [
                  '-lz',
                ],
              },
            }], # condition #2, OS == "android"

          ], # condtions
        }], # use_system_zlib == 0
      ], # conditions
    }, # target
    {
      'target_name': 'minizip',
      'type': 'static_library',
      'dependencies': [
        'zlib',
      ],
      'conditions': [
        ['use_system_minizip == 0', {
          'direct_dependent_settings': {
            'include_dirs': [
              '../..', # for third_party/zlib/zlib.h
              'contrib/minizip',
            ],
          },
          'include_dirs': [
            '../..',
          ],
          'sources': [
            'contrib/minizip/ioapi.c',
            'contrib/minizip/ioapi.h',
            'contrib/minizip/iowin32.c',
            'contrib/minizip/iowin32.h',
            'contrib/minizip/unzip.c',
            'contrib/minizip/unzip.h',
            'contrib/minizip/zip.c',
            'contrib/minizip/zip.h',
          ],
          'conditions': [
            ['OS != "win"', {
              'sources!': [
                'contrib/minizip/iowin32.c'
              ],
            }],
          ],
        }, { # use_system_minizip != 0
          'direct_dependent_settings': {
            'defines': [
              'USE_SYSTEM_MINIZIP',
            ],
          },
          'defines': [
            'USE_SYSTEM_MINIZIP',
          ],
          'link_settings': {
            'libraries': [
              '-lminizip',
            ],
          },
        }], # use_system_minizip

        ['OS=="mac" or OS=="ios" or os_bsd==1 or OS=="android"', {
          # Mac, Android and the BSDs don't have fopen64, ftello64, or
          # fseeko64. We use fopen, ftell, and fseek instead on these
          # systems.
          'defines': [
            'USE_FILE32API'
          ],
        }],

        ['clang==1', {
          'xcode_settings': {
            'WARNING_CFLAGS': [
              # zlib uses `if ((a == b))` for some reason.
              '-Wno-parentheses-equality',
            ],
          },
          'cflags': [
            '-Wno-parentheses-equality',
          ],
        }],
      ],
    }
  ],
}
