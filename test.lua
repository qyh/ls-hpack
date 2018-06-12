local hpack = require "hpack"

local pack = hpack.new()

print(pack)

local r = pack:encode("content-length", "2145")
print(pack:decode(r))

local headers = {}
headers["content-length"] = 214
headers["etag"] = "sdjfsxxxxxxxxxxjieuiecner53453jvxjsj3i5wwerwjksjdfj"
local result
for k, v in pairs(headers) do
    local r, l = pack:encode(k, v)
    print(#r, l, pack:decode(r))
    result = result and result..r or r
end

--print(#result, pack:batch_decode(result))
local t = pack:batch_decode(result)
print(t["etag"])
print(t["content-length"])
