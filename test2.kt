import java.io.File
import kotlin.system.measureTimeMillis

suspend fun main() {
    val filePath = "test_io_file.txt"
    val testData = "This is a line of text to write into file.\n"

    // Prepare the file
    val file = File(filePath)
    file.createNewFile()

    // Perform write test
    repeat(100) { index ->
        val timeTaken = measureTimeMillis {
            file.appendText("$testData Line: $index\n")
        }
        println("Write operation $index took $timeTaken ms")
    }

    // Perform read test
    repeat(100) { index ->
        val timeTaken = measureTimeMillis {
            val lines = file.readLines()
            println("Read operation $index, lines read: ${lines.size}")
        }
        println("Read operation $index took $timeTaken ms")
    }

    // Cleanup
    file.delete()
}
