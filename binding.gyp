{
  'targets': [
    {
      'target_name': 'xlsxio',
      'sources': [
        'cpp/checkLibrary.cc',
        'cpp/third-party/xlsxio/lib/xlsxio_read.c',
        'cpp/third-party/xlsxio/lib/xlsxio_read_sharedstrings.c',
        'cpp/third-party/xlsxio/lib/xlsxio_write.c',
      ],
      'include_dirs': [
        '<!(node -e "require(\'nan\')")',
        'cpp/third-party/xlsxio/include',
        'cpp/third-party/xlsxio/lib',
        'cpp/third-party/zlib-gyp',
        'cpp/third-party/libexpat',
      ],
      'dependencies': [
        'cpp/third-party/libexpat/libexpat.gyp:expat',
        'cpp/third-party/zlib-gyp/zlib.gyp:zlib'
      ]
    }
  ]
}
