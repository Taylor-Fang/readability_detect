该程序用于检测字符串是否可读，如google，facebook等等具有可读性，而随机字符串如ahneimsfks,mgnjainfwses等不具有可读性。



原理：基于马尔可夫链模型来判断字符串的可读性。通过对语料库(http://norvig.com/big.txt)的统计，计算每个字符下一字符出
现的自然对数概率，即ln(P(bigram)) (使用自然对数概率，主要为避免小数溢出问题)，建立马尔可夫转移概率矩阵。通过该概率
矩阵计算字符串的可读性概率，如果概率大于所设阈值c，则认为可读，否则则不可读。


计算：设字符串的相邻双子符的组合为{bigram1,bigram2,...bigramn},如google：{go,oo,og,gl,le}
由于每个字符出现的下一字符与该字符无关，所以相互独立，因此字符串的可读性概率
				p=P(bigram1,...,bigramn)=P(bigram1)*...*P(bigramn)
				
				
				
由于P(bigram)是一个很小的小数，所以p为概率的乘积可能会导致计算机无法表示表示而导致小数溢出，所以这里用自然对数概率表示
即：			ln(p)=ln(P(bigram1))+...+ln(P(bigramn))