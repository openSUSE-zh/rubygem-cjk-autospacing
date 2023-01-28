require "minitest/autorun"
require "cjk_auto_space"

class CjkAutoSpaceTest < Minitest::Test
  def test_auto_space
    assert_equal "你好 world 哈哈",
      "你好world哈哈".cjk_auto_space.force_encoding("UTF-8")
  end
 
  def test_auto_space!
    assert_equal "你好 world 哈哈",
      "你好world哈哈".cjk_auto_space!.force_encoding("UTF-8")
  end
end
