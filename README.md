rubygem-cjk_auto_space
------

A rubygem that automatically add sapce(U+0020) between CJK and other unicode code points, for better reading experience, with Markdown syntax concerned.

## Usage

  "你好world哈哈".cjk_auto_space // => "你好 world 哈哈"

## Benchmark

    ruby benchmark_cjk_auto_space.rb 
           user     system      total        real
    autocorrect-rb  0.065535   0.019145   0.084680 (  0.085009)
    cjk_auto_space  0.008270   0.004089   0.012359 (  0.012361)
