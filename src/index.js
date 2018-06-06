const xlsxUtil = require('bindings')('xlsx-util.node')

console.log('This should be eight:', xlsxUtil.parseXlsx(7, 5));
