.pragma library

/************** Adds most of QString/QByteArray API to JS String *******************************/

function addMethod (object, method) {
    Object.defineProperty (object, method.name, {
                               "value" : method,
                               "enumerable" : false
                           });
}


function at (idx) {
    return (this [idx]);
}
addMethod (Array.prototype,  at);
addMethod (String.prototype, at);


function first () {
    return this [0];
}
addMethod (Array.prototype,  first);
addMethod (String.prototype, first);


function last () {
    return (this.length ? this [this.length -1] : undefined);
}
addMethod (Array.prototype,  last);
addMethod (String.prototype, last);


function contains (value) {
    return (this.indexOf (value) > -1);
}
addMethod (Array.prototype,  contains);
addMethod (String.prototype, contains);


function size () {
    return (this.length);
}
addMethod (Array.prototype,  size);
addMethod (String.prototype, size);


function isEmpty () {
    return (!this.length);
}
addMethod (Array.prototype,  isEmpty);
addMethod (String.prototype, isEmpty);


function clear () {
    this.splice (0, this.length);
}
addMethod (Array.prototype, clear);


function foreach (callback) {
    for (var i = 0; i < this.length; callback (i, this [i]), i++);
}
addMethod (Array.prototype, foreach);


function append (value) {
    this.push (value);
}
addMethod (Array.prototype, append);


function prepend (value) {
    this.unshift (value);
}
addMethod (Array.prototype, prepend);


function removeAt (idx) {
    this.splice (idx, 1);
}
addMethod (Array.prototype, removeAt);


function removeAll (value) {
    var ret = 0;
    var it = 0;
    while (it < this.length) {
        if (this [it] === (value)) {
            this.splice (it, 1);
            ret++;
        }
        else { it++; }
    }
    return ret;
}
addMethod (Array.prototype, removeAll);


function takeAt (idx) {
    var ret = this [idx];
    this.splice (idx, 1);
    return ret;
}
addMethod (Array.prototype, takeAt);


function erase (idx) {
    this [idx] = undefined;
}
addMethod (Array.prototype, erase);


function swap (idx1, idx2) {
    var tmp = this [idx1];
    this [idx1] = this [idx2];
    this [idx2] = tmp;
}
addMethod (Array.prototype, swap);


function replace (idx, value) {
    this [idx] = value;
}
addMethod (Array.prototype, replace);


function squeeze () {
    var it = 0;
    while (it < this.length) {
        if (this [it] !== null && this [it] !== undefined) {
            this.splice (it, 1);
        }
        else { it++; }
    }
}
addMethod (Array.prototype, squeeze);


function append (str) {
    return String (this + (str || ""));
}
addMethod (String.prototype, append);


function prepend (str) {
    return String ((str || "") + this);
}
addMethod (String.prototype, prepend);


function trimmed () {
    return String (this.replace (/\s*$/, '').replace (/^\s*/, ''));
}
addMethod (String.prototype, trimmed);


function remove (str) {
    var ret = String (this);
    while (ret.indexOf (str) > -1) {
        ret = ret.replace (str, '');
    }
    return ret;
}
addMethod (String.prototype, remove);


function repeat (str, count) {
    return (new Array (count +1).join (str));
}
addMethod (String, repeat);


function mid (pos, len) {
    return this.substr (pos, len);
}
addMethod (String.prototype, mid);


function left (len) {
    return (len < this.length ? this.substr (0, len) : this);
}
addMethod (String.prototype, left);


function right (len) {
    return (len < this.length ? this.substr (this.length - len, len) : this);
}
addMethod (String.prototype, right);


function startsWith (str) {
    return (str && this.indexOf (str) === 0);
}
addMethod (String.prototype, startsWith);


function endsWith (str) {
    return (str && this.lastIndexOf (str) > -1 && this.lastIndexOf (str) === (this.length - str.length));
}
addMethod (String.prototype, endsWith);


function toBase64 () {
    return Qt.atob (this);
}
addMethod (String.prototype, toBase64);


function fromBase64 (str) {
    return Qt.btoa (str);
}
addMethod (String, fromBase64);

