# Simple ML language demo program

# Define a function to calculate the sum of a list
fun sum(list)
    var total = 0
    for i = 0 to len(list) - 1 then
        total = total + list[i]
    end
    return total
end

# Define a function to calculate the mean of a list
fun mean(list)
    return sum(list) / len(list)
end

# Create a list of numbers
var data = [10, 20, 30, 40, 50]

# Calculate and print the sum and mean
print("Data: ", data)
print("Sum: ", sum(data))
print("Mean: ", mean(data))

# Demonstrate a simple ML operation
var embedded = EMBED "This is a text that will be embedded"
print("Embedding: ", embedded)

# Demonstrate an AI model call
var prediction = AI simple_model("What is the weather like today?")
print("AI response: ", prediction)
