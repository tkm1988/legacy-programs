class Fibo(n:Int = 2)
{
  private var _fibo1: Double = 1.0
  private var _fibo2: Double = 2.0
  private var _fibo3: Double = 0.0
  private var _Memo = List(_fibo1, _fibo1, _fibo2)
  
  if(n <= 2 && n >= 0){
    for(i: Int <- Range(0,2-n)){
      _Memo = _Memo.init
    }
  } else {
    for(i: Int <- Range(0,n-2)){
      _fibo3 = _fibo2 + _fibo1
      _fibo1 = _fibo2
      _fibo2 = _fibo3
      _Memo = _Memo ::: List(_fibo3)
    }
  }

  def Memo = _Memo

  def this(maxVal: Double = 2.0){
    this(2)
    while(_Memo.last < maxVal){
      _fibo3 = _fibo2 + _fibo1
      _fibo1 = _fibo2
      _fibo2 = _fibo3
      _Memo = _Memo ::: List(_fibo3)
    }
    _Memo = _Memo.init
  }
}

object ans
{
  def main(args: Array[String])
  {
    var sum: Double = 0.0
    val f = new Fibo(4000000.0)
    println(f.Memo)
    for(n: Double <- f.Memo.filter(_%2.0 == 0))
      sum += n
    println(sum)
  }
}
