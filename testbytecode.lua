-- This is to test the pure lua bytecode loader
local x = 10
local y = 20
local z = x + y
print(z)

local function test()
    print("test")
end
function test2()
    print("test2")
end
zz = z
loadstring("print(zz)")()
test()
test2()
for i = 1, 10 do
    print(i)
end
while true do
    print("while")
    break
end
repeat
    print("repeat")
until true
if true then
    print("if")
end
if false then
    print("if")
else
    print("else")
end
if false then
    print("if")
elseif true then
    print("elseif")
else
    print("else")
end
x = 10
y = 20
x,y=y,x
for _ = #x, 1, -1 do
end