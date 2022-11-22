use proc_macro::{Span, TokenStream};
use quote::ToTokens;
use syn::{
    parse_macro_input, punctuated::Punctuated, Ident, ImplItem, ImplItemConst, Item, Path,
    TraitItem, TraitItemConst,
};

pub fn vtable(input: TokenStream) -> TokenStream {
    let item = parse_macro_input!(input as Item);

    let output = match item {
        Item::Impl(impl_) if impl_.trait_.is_none() => syn::Error::new(
            Span::call_site().into(),
            "`#[vtable]` on impl must be associated with a vtabled trait",
        )
        .into_compile_error(),
        Item::Impl(mut impl_) => {
            let mut has_consts = vec![];

            for item in &impl_.items {
                if let ImplItem::Method(method_) = item {
                    has_consts.push(ImplItem::Const(ImplItemConst {
                        vis: syn::Visibility::Inherited,
                        ident: Ident::new(
                            &format!("HAS_{}", method_.sig.ident.to_string().to_ascii_uppercase()),
                            method_.sig.ident.span(),
                        ),
                        expr: syn::Expr::Lit(syn::ExprLit {
                            lit: syn::Lit::Bool(syn::LitBool {
                                value: true,
                                span: method_.sig.ident.span(),
                            }),
                            attrs: vec![],
                        }),
                        ty: syn::Type::Path(syn::TypePath {
                            qself: None,
                            path: Path {
                                leading_colon: Default::default(),
                                segments: {
                                    let mut pg = Punctuated::new();
                                    pg.push_value(syn::PathSegment {
                                        ident: Ident::new("bool", method_.sig.ident.span()),
                                        arguments: syn::PathArguments::None,
                                    });
                                    pg
                                },
                            },
                        }),
                        attrs: vec![],
                        defaultness: None,
                        const_token: Default::default(),
                        colon_token: Default::default(),
                        eq_token: Default::default(),
                        semi_token: Default::default(),
                    }));
                }
            }

            impl_.items.extend(has_consts);
            impl_.into_token_stream()
        }
        Item::Trait(mut trait_) => {
            let mut has_consts = vec![];

            for item in &trait_.items {
                if let TraitItem::Method(method_) = item {
                    has_consts.push(TraitItem::Const(TraitItemConst {
                        ident: Ident::new(
                            &format!("HAS_{}", method_.sig.ident.to_string().to_ascii_uppercase()),
                            method_.sig.ident.span(),
                        ),
                        default: Some((
                            Default::default(),
                            syn::Expr::Lit(syn::ExprLit {
                                lit: syn::Lit::Bool(syn::LitBool {
                                    value: false,
                                    span: method_.sig.ident.span(),
                                }),
                                attrs: vec![],
                            }),
                        )),
                        ty: syn::Type::Path(syn::TypePath {
                            qself: None,
                            path: Path {
                                leading_colon: Default::default(),
                                segments: {
                                    let mut pg = Punctuated::new();
                                    pg.push_value(syn::PathSegment {
                                        ident: Ident::new("bool", method_.sig.ident.span()),
                                        arguments: syn::PathArguments::None,
                                    });
                                    pg
                                },
                            },
                        }),
                        attrs: vec![],
                        const_token: Default::default(),
                        colon_token: Default::default(),
                        semi_token: Default::default(),
                    }));
                }
            }

            trait_.items.extend(has_consts);
            trait_.into_token_stream()
        }
        _ => syn::Error::new(
            Span::call_site().into(),
            "`#[vtable]` can only be applied to `trait` and `impl`",
        )
        .into_compile_error(),
    };

    TokenStream::from(output)
}
