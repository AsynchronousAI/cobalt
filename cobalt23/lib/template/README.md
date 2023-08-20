# cobalt23/lib/template
Template is a text generation engine. 

Good for generating C, HTML, or Cobalt code. Used in the Cobalt compiler and tobeadded AST library.
## Usage
```js
var template = import 'template'
```
```lua
fields = {
	{a = 'int'},
	{b = 'const char*'},
	{c = 'double'},
}
print(template([[
typedef struct {
<? for (_,name_ctype in ipairs(fields)) {
	var name, ctype = next(name_ctype);
?>	<?=ctype?> <?=name?>;
<? }
?>} S;
]], {
	fields = fields,
}))
```
