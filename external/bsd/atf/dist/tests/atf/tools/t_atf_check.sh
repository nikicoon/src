#
# Automated Testing Framework (atf)
#
# Copyright (c) 2008, 2009 The NetBSD Foundation, Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND
# CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
# INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
# GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
# IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
# IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

h_pass()
{
    cmd="$1"; shift

    echo "Running [atf-check $*] against [${cmd}]"

    cat >script.sh <<EOF
#! $(atf-config -t atf_shell)
${cmd}
EOF
    chmod +x script.sh

    if ! atf-check "${@}" ./script.sh >tmp; then
        cat tmp
        atf_fail "atf-check failed"
    fi
}

h_fail()
{
    cmd="$1"; shift

    echo "Running [atf-check $*] against [${cmd}]"

    cat >script.sh <<EOF
#! $(atf-config -t atf_shell)
${cmd}
EOF
    chmod +x script.sh

    if atf-check "${@}" ./script.sh 2>tmp; then
        cat tmp
        atf_fail "atf-check succeeded but should fail"
    fi
}

atf_test_case sflag
sflag_head()
{
    atf_set "descr" "Tests for the -s option"
}
sflag_body()
{
    h_pass "true" -s eq:0
    h_pass "false" -s ne:0
    h_pass "exit 255" -s eq:255
    h_pass "exit 0" -s ne:255

    h_fail "exit 256" -s eq:256
    h_fail "exit -1" -s eq:-1
    h_fail "true" -s ne:256
    h_fail "true" -s ne:-1
}

atf_test_case xflag
xflag_head()
{
    atf_set "descr" "Tests for the -x option"
}
xflag_body()
{
    atf-check -s ne:0 -o ignore -e ignore "echo foo 2>&1" || \
        atf_fail "Shell command succeeded without -x"

    atf-check -e inline:"foo\n" -x "echo foo 1>&2" || \
        atf_fail "Cannot run command with -x"

    atf-check -o inline:"foo\n" -x echo foo || \
        atf_fail "Using -x does not respect all provided arguments"
}

atf_test_case oflag_empty
oflag_empty_head()
{
    atf_set "descr" "Tests for the -o option using the 'empty' argument"
}
oflag_empty_body()
{
    h_pass "true" -o empty
    h_fail "echo foo" -o empty
}

atf_test_case oflag_ignore
oflag_ignore_head()
{
    atf_set "descr" "Tests for the -o option using the 'ignore' argument"
}
oflag_ignore_body()
{
    h_pass "true" -o ignore
    h_pass "echo foo" -o ignore
}

atf_test_case oflag_file
oflag_file_head()
{
    atf_set "descr" "Tests for the -o option using the 'file:' argument"
}
oflag_file_body()
{
    touch empty
    h_pass "true" -o file:empty

    echo foo >text
    h_pass "echo foo" -o file:text
    h_fail "echo bar" -o file:text

    dd if=/dev/urandom of=bin bs=1k count=10
    h_pass "cat bin" -o file:bin
}

atf_test_case oflag_inline
oflag_inline_head()
{
    atf_set "descr" "Tests for the -o option using the 'inline:' argument"
}
oflag_inline_body()
{
    h_pass "true" -o inline:
    h_pass "echo foo bar" -o inline:"foo bar\n"
    h_pass "printf 'foo bar'" -o inline:"foo bar"
    h_pass "printf '\t\n\t\n'" -o inline:"\t\n\t\n"
    # XXX Ugly hack to workaround the lack of \e in FreeBSD.  Look for a
    # nicer solution...
    case $(uname) in
    FreeBSD)
        h_pass "printf '\a\b\f\n\r\t\v'" -o inline:"\a\b\f\n\r\t\v"
        ;;
    *)
        h_pass "printf '\a\b\e\f\n\r\t\v'" -o inline:"\a\b\e\f\n\r\t\v"
        ;;
    esac
    h_pass "printf '\011\022\033\012'" -o inline:"\011\022\033\012"

    h_fail "echo foo bar" -o inline:"foo bar"
    h_fail "echo -n foo bar" -o inline:"foo bar\n"
}

atf_test_case oflag_save
oflag_save_head()
{
    atf_set "descr" "Tests for the -o option using the 'save:' argument"
}
oflag_save_body()
{
    h_pass "echo foo" -o save:out
    echo foo >exp
    cmp -s out exp || atf_fail "Saved output does not match expected results"
}

atf_test_case eflag_empty
eflag_empty_head()
{
    atf_set "descr" "Tests for the -e option using the 'empty' argument"
}
eflag_empty_body()
{
    h_pass "true 1>&2" -e empty
    h_fail "echo foo 1>&2" -e empty
}

atf_test_case eflag_ignore
eflag_ignore_head()
{
    atf_set "descr" "Tests for the -e option using the 'ignore' argument"
}
eflag_ignore_body()
{
    h_pass "true 1>&2" -e ignore
    h_pass "echo foo 1>&2" -e ignore
}

atf_test_case eflag_file
eflag_file_head()
{
    atf_set "descr" "Tests for the -e option using the 'file:' argument"
}
eflag_file_body()
{
    touch empty
    h_pass "true 1>&2" -e file:empty

    echo foo >text
    h_pass "echo foo 1>&2" -e file:text
    h_fail "echo bar 1>&2" -e file:text

    dd if=/dev/urandom of=bin bs=1k count=10
    h_pass "cat bin 1>&2" -e file:bin
}

atf_test_case eflag_inline
eflag_inline_head()
{
    atf_set "descr" "Tests for the -e option using the 'inline:' argument"
}
eflag_inline_body()
{
    h_pass "true 1>&2" -e inline:
    h_pass "echo foo bar 1>&2" -e inline:"foo bar\n"
    h_pass "printf 'foo bar' 1>&2" -e inline:"foo bar"
    h_pass "printf '\t\n\t\n' 1>&2" -e inline:"\t\n\t\n"
    # XXX Ugly hack to workaround the lack of \e in FreeBSD.  Look for a
    # nicer solution...
    case $(uname) in
    FreeBSD)
        h_pass "printf '\a\b\f\n\r\t\v' 1>&2" -e inline:"\a\b\f\n\r\t\v"
        ;;
    *)
        h_pass "printf '\a\b\e\f\n\r\t\v' 1>&2" -e inline:"\a\b\e\f\n\r\t\v"
        ;;
    esac
    h_pass "printf '\011\022\033\012' 1>&2" -e inline:"\011\022\033\012"

    h_fail "echo foo bar 1>&2" -e inline:"foo bar"
    h_fail "echo -n foo bar 1>&2" -e inline:"foo bar\n"
}

atf_test_case eflag_save
eflag_save_head()
{
    atf_set "descr" "Tests for the -e option using the 'save:' argument"
}
eflag_save_body()
{
    h_pass "echo foo 1>&2" -e save:out
    echo foo >exp
    cmp -s out exp || atf_fail "Saved output does not match expected results"
}

atf_init_test_cases()
{
    atf_add_test_case sflag
    atf_add_test_case xflag

    atf_add_test_case oflag_empty
    atf_add_test_case oflag_ignore
    atf_add_test_case oflag_file
    atf_add_test_case oflag_inline
    atf_add_test_case oflag_save

    atf_add_test_case eflag_empty
    atf_add_test_case eflag_ignore
    atf_add_test_case eflag_file
    atf_add_test_case eflag_inline
    atf_add_test_case eflag_save
}

# vim: syntax=sh:expandtab:shiftwidth=4:softtabstop=4
