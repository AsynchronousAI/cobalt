--[[
  Cobalt Preprocessor Language Expansion
  - Provides a typechecker
  - Provides global variables `global var` or `global function`
]]

-- TYPES
types = {
  "string" = function(item)
    -- Check if item is a string
    return type(item) == "string"
  end,
  "number" = function(item)
    -- Check if item is a number
    return type(item) == "number"
  end,
  "boolean" = function(item)
    -- Check if item is a boolean
    return type(item) == "boolean"
  end,
  "table" = function(item)
    -- Check if item is a table
    return type(item) == "table"
  end,
  "function" = function(item)
    -- Check if item is a function
    return type(item) == "function"
  end,
  "userdata" = function(item)
    -- Check if item is a userdata
    return type(item) == "userdata"
  end,
  "thread" = function(item)
    -- Check if item is a thread
    return type(item) == "thread"
  end,
  "null" = function(item)
    -- Check if item is null
    return type(item) == "null"
  end,
  "any" = function(item)
    -- Check if item is any type
    return true
  end,
  "int" = function(item)
    -- Check if item is an integer
    return math.floor(item) == item
  end,
  "float" = function(item)
    -- Check if item is a float
    return math.floor(item) ~= item
  end,
  "double" = function(item)
    -- Check if item is a double
    return math.floor(item) ~= item
  end,
}

-- PARSER
function def(str)
    -- Takes in code and looks for definitions with annotations
    local pattern = "^%s*var%s+([^%s]+)%s*:%s*([^%s]+)%s*=%s*([^%s]+)%s*$"
    local result = {}
    for line in str:gmatch("[^\r\n]+") do
      local var, key, value = line:match(pattern)
      if var and key and value then
        table.insert(result, {var, key, value})
      end
    end
    return result
end
function assign(str)
    -- Takes in code looks for assignments with annotations
    local pattern = "%s*([^%s]+)%s*:%s*([^%s]+)%s*=%s*([^%s]+)%s*"
    local result = {}
    for line in str:gmatch("[^\r\n]+") do
      local var, key, value = line:match(pattern)
      if var and key and value then
        -- Verify var, key, value are not eligible for def
        if not def(line)[1] then
            table.insert(result, {var, key, value})
        end
      end
    end
    return result
end
function clean(str)
  -- Takes in code and removes comments, strings.
  return str:gsub('"[^"]-"', ''):gsub("'[^']-'", ''):gsub('%[%[.-%]%]', ''):gsub('//.-\n', ''):gsub('/%*.-%*/', '')
end

-- CHECKER
function check(type, value)
  value = loadstring("return " .. value)()
  if types[type] then
    return types[type](value)
  else
    error()
  end
end
-- Example usage
local str = [[
a: b = 1
c:d=2
e : f = 3
var x: g = 4
h : i = 5
]]
local pairs = assign(clean(str))
for i, pair in ipairs(pairs) do
    print(pair[1], pair[2], pair[3])
end