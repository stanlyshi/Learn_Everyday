SipHash：快速的短输入PRF
SipHash是一系列伪随机函数（又名键控散列函数），它们针对短消息的速度进行了优化。
------------------------------------

https://www.131002.net/siphash/

------------------------------------

目标应用程序包括网络流量身份验证和 针对散列式 DoS攻击的防御。

SipHash安全，快速，简单（实际）：
SipHash比以前的加密算法（例如，基于通用哈希的MAC）更简单，更快。
SipHash在不安全的 非加密算法（例如MurmurHash）的 性能方面具有竞争力
我们建议哈希表作为哈希函数切换到SipHash。 SipHash的用户已经包括FreeBSD，OpenDNS，Perl 5，Ruby或Rust。

原始的SipHash返回64位字符串。后来根据用户需求创建了一个返回128位字符串的版本。

知识产权： 我们不知道有任何与SipHash相关的专利或专利申请，并且我们不打算申请任何专利。SipHash的参考代码以CC0许可证（类似于公共领域的许可证）发布。

SipHash是由
Jean-Philippe Aumasson（瑞士Kudelski Security）
Daniel J. Bernstein（伊利诺伊大学，美国芝加哥）
联系人：jeanphilippe.aumasson@gmail.com   djb@cr.yp.to
