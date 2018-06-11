{
  'targets': [
    {
      'target_name': 'xlsxio',
      'sources': [
        'cpp/init.cpp',
        'cpp/checkLibrary.cc',
        'cpp/getSheetNames.cpp',
        'cpp/NapiAsync.cpp',
        'cpp/xlsx.cpp',
        'cpp/third-party/xlsxio/lib/xlsxio_read.c',
        'cpp/third-party/xlsxio/lib/xlsxio_read_sharedstrings.c',
        'cpp/third-party/xlsxio/lib/xlsxio_write.c',
      ],
      'defines': [ 'USE_MINIZIP' ],
      'include_dirs': [
        '<!(node -e "require(\'nan\')")',
        'cpp/third-party/xlsxio/include',
        'cpp/third-party/xlsxio/lib',
        'cpp/third-party/zlib-gyp/contrib/minizip',
        'cpp/third-party/zlib-gyp/contrib',
        'cpp/third-party/libexpat',
        "<!@(node -p \"require('node-addon-api').include\")",
      ],
      'dependencies': [
        'cpp/third-party/libexpat/libexpat.gyp:expat',
        "<!(node -p \"require('node-addon-api').gyp\")",
      ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'xcode_settings': {
        'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
        'CLANG_CXX_LIBRARY': 'libc++',
        'MACOSX_DEPLOYMENT_TARGET': '10.7',
      },
      'msvs_settings': {
        'VCCLCompilerTool': { 'ExceptionHandling': 1 },
      },
    }
  ]
}
