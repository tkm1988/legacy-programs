class palindromicNumber(n : Int = 0)
{
  private var _mid : Int = (n.toString).size / 2
  private var _strLeft : String = (n.toString).take(_mid)
  private var _strRight : String = ((n.toString).takeRight(_mid)).reverse

  def Check:Boolean = _strLeft == _strRight

  def Check(m : Int):Boolean ={
    _mid = (m.toString).size / 2
    _strLeft = (m.toString).take(_mid)
    _strRight = ((m.toString).takeRight(_mid)).reverse
    return Check
  }
}

object ans
{
  def main(args : Array[String])
  {
    var Result = List.empty[Int]
    var pn = new palindromicNumber()
    
    for(i: Int <- Range(1, 1000)){
      for(j: Int <- Range(i, 1000)){
        if(pn.Check(i * j)){
          Result = Result ::: List(i*j)
          print("*\t")
        }
        println(i + " x " + j + " = " + (i*j))
      }
    }
    println(Result)
  }
}
