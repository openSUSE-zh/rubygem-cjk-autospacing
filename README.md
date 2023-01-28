rubygem-cjk_auto_space
------

A rubygem that automatically add sapce(U+0020) between CJK and other unicode code points, for better reading experience, with Markdown syntax concerned.

## Usage

  "你好world哈哈".cjk_auto_space // => "你好 world 哈哈"

## Benchmark

titles.txt

    ruby benchmark_cjk_auto_space.rb 
                       user     system      total        real
    autocorrect-rb  0.065535   0.019145   0.084680 (  0.085009)
    cjk_auto_space  0.008270   0.004089   0.012359 (  0.012361)

posts.txt

    ruby benchmark_cjk_auto_space.rb 
                       user     system      total        real
    autocorrect-rb 11.361764   0.012041  11.373805 ( 11.379011)
    cjk_auto_space  9.522672   0.260081   9.782753 (  9.785318)
