import scala.util.Random

class LCG(M : Long = 0L)
{
  private val _r = new Random()
  private val _M : Long = if(M == 0L) _r.nextLong() else M
  private val _A : Long = _r.nextLong() % (_M-2L)
  private val _B : Long = _r.nextLong() % (_M-3L)

  def nextLong(x : Long) = (_A * x + _B) % _M
}

class Factorization(n:Long = 2L)
{
  private val _n = n
  private var _Result: Long = 0

  def Result = _Result

  def GCD(a: Long, b: Long):Long = if(b == 0L) a else GCD(b, a%b)
  
  def PollardsRho = {
    var x : Long = 2L
    var y : Long = 2L
    var d : Long = 1L
    val fx = new LCG(_n)

    while(d == 1){
      x = fx.nextLong(x)
      y = fx.nextLong(fx.nextLong(y))

      d = GCD(Math.abs(x - y), n)
    }

    d
  }
}

object ans
{
  def main(args: Array[String])
  {
    var n : Long = 600851475143L
    var d : Long = 0L
    var Result = List.empty[Long]

    do{
      var f = new Factorization(n)
      d = f.PollardsRho
      n /= d
      Result = Result ::: List(d)
    }while(n > d)

    println(Result)
  }
}
