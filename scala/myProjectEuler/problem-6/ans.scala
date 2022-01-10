object ans
{
  def main(args : Array[String])
  {
    val start : Int = 1
    val end : Int = 100
    var sum1 : Int = (start to end).sum
    var sum2 : Int = ((start to end).map(x=>x*x)).sum

    println(sum1 * sum1 - sum2)

  }
}
