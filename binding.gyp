{
  'targets': [
    {
      'target_name': 'blerp',
      'sources': [ 'cpp/hello.cc' ],
      'include_dirs': [
        '<!(node -e "require(\'nan\')")'
      ],
      'dependencies': [
        'cpp/third-party/libexpat/libexpat.gyp:expat'
      ]
    }
  ]
}
