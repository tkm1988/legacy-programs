object Ans {
  def main(args: Array[String])
  {
    var sum: Int = 0
    
    for(n:Int <- Range(3, 1000))
      if(((n%3) == 0)||((n%5) == 0))
        sum += n

    println(sum)
  }
}

