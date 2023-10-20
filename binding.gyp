{
  'targets': [
    {
      'target_name': 'lock',
      'sources':     ['cpp/lock.cpp'],
      'cflags_cc':   ['-fexceptions' ],
      'cflags!':     [ '-fno-exceptions' ],
      'cflags_cc!':  [ '-fno-exceptions' ],
      'conditions':  [
        [
          'OS=="win"',
          {
            'msvs_settings' : {
              'VCCLCompilerTool': {
                'AdditionalOptions': ["/EHsc", "/execution-charset:utf-8", "/source-charset:utf-8"]
              }
            }
          }
        ],
        ['OS=="mac"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
          }
        }]
      ]
    }
  ]
}
