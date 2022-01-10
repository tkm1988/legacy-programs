object ans
{
  def gcd(a : Long, b : Long):Long = if(b == 0L) a else gcd(b, a%b)
  def lcm(n : Long, m : Long, max : Long): Long = if(max == 0L) n * m / gcd(n, m) else n * lcm(m, m+1L, max-1L) / gcd(n, lcm(m, m+1L, max-1L))

  def main(args : Array[String])
  {
    println(lcm(1L,2L,18L))
  }
}


